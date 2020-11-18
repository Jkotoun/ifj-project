/****
 * Implementace překladače imperativního jazyka IFJ20.
 * Jiří Vlasák (xvlasa15)
 * Parser
 * CONVENTIONS:
 * 1: every rule expects its first token to be already loaded in current_token variable.
 * 2: rules that read 1 token into next rule end with suffix "_next".
*/

#include "parser.h"
#include "parser_rules.h"
#include "parser_helpers.h"
#include "scanner.h"
#include "symtable.h"
#include "error_codes.h"
#include "queue.h"
#include "expression.h"
#include <string.h>
#include "dl_list.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define assert_token_is(token) func_assert_token_is(token, __func__)
#define assert_keyword_is(keyword) func_assert_keyword_is(keyword, __func__)
#define assert_true(expression, errno) func_assert_true(expression, errno, __func__)

#define handle_error(errno) func_handle_error(errno, __func__)

token current_token;

symbol_node* functions_symtable;
symbol_node* current_function;

bool functionHasReturn;

tDLList scoped_symtables;
typeQueue typeQ;
tokenQueue tokenQ;
tokenQueue exprTokenQ;

token* leftTokenArr;
int leftSideLength;

int rightSideLength;
// node rootNode;
// tDLList list;

void init_builtins()
{
    string func_name;
    strInit(&func_name);


    varType returnArr[] = { STRING, INT };
    strAddConstStr(&func_name, "inputs");
    symtable_insert_node_func(&functions_symtable, &func_name, 2, returnArr, 0, NULL, true);
    strClear(&func_name);


    returnArr[0] = INT;
    strAddConstStr(&func_name, "inputi");
    symtable_insert_node_func(&functions_symtable, &func_name, 2, returnArr, 0, NULL, true);
    strClear(&func_name);

    returnArr[0] = FLOAT;
    strAddConstStr(&func_name, "inputf");
    symtable_insert_node_func(&functions_symtable, &func_name, 2, returnArr, 0, NULL, true);
    strClear(&func_name);

    strAddConstStr(&func_name, "print");
    symtable_insert_node_func(&functions_symtable, &func_name, 0, NULL, -1, NULL, true);
    strClear(&func_name);

    varType paramArr[] = { INT, INT, INT };
    returnArr[0] = FLOAT;
    strAddConstStr(&func_name, "int2float");
    symtable_insert_node_func(&functions_symtable, &func_name, 1, returnArr, 1, paramArr, true);
    strClear(&func_name);

    paramArr[0] = FLOAT;
    returnArr[0] = INT;
    strAddConstStr(&func_name, "float2int");
    symtable_insert_node_func(&functions_symtable, &func_name, 1, returnArr, 1, paramArr, true);
    strClear(&func_name);

    paramArr[0] = STRING;
    returnArr[0] = INT;
    strAddConstStr(&func_name, "len");
    symtable_insert_node_func(&functions_symtable, &func_name, 1, returnArr, 1, paramArr, true);
    strClear(&func_name);

    paramArr[0] = STRING;
    paramArr[1] = INT;
    paramArr[2] = INT;
    returnArr[0] = STRING;
    returnArr[1] = INT;
    strAddConstStr(&func_name, "substr");
    symtable_insert_node_func(&functions_symtable, &func_name, 2, returnArr, 3, paramArr, true);
    strClear(&func_name);

    paramArr[0] = STRING;
    paramArr[1] = INT;
    returnArr[0] = INT;
    returnArr[1] = INT;
    strAddConstStr(&func_name, "ord");
    symtable_insert_node_func(&functions_symtable, &func_name, 2, returnArr, 2, paramArr, true);
    strClear(&func_name);

    paramArr[0] = INT;
    returnArr[0] = STRING;
    returnArr[1] = INT;
    strAddConstStr(&func_name, "chr");
    symtable_insert_node_func(&functions_symtable, &func_name, 2, returnArr, 1, paramArr, true);
    strClear(&func_name);

    strFree(&func_name);
}

void parser_start()
{
    string str, rawStr;
    strInit(&str);
    strInit(&rawStr);

    current_token.str = &str;
    current_token.token_str_raw = &rawStr;

    symtable_init(&functions_symtable);
    typeQueueInit(&typeQ);

    DLInitList(&scoped_symtables);
    init_builtins();

    rule_prog();

    symbol_node* main_node;
    string main_str;
    strInit(&main_str);
    strAddChar(&main_str, 'm');
    strAddChar(&main_str, 'a');
    strAddChar(&main_str, 'i');
    strAddChar(&main_str, 'n');

    if (!symtable_search(&functions_symtable, &main_str, &main_node)) //main not defined
    {
        handle_error(OTHER_SEMANTIC_ERR);
    }

    if (symtable_contains_undef_func(&functions_symtable))
    {
        handle_error(VAR_DEFINITION_ERR);
    }
    exit(0);
}

void rule_prog()
{
    get_next_token();
    rule_eols_next();

    rule_prolog();
    get_next_token();
    assert_token_is(EOL_TOKEN);

    get_next_token();
    rule_eols_next();

    rule_func_decl();

    get_next_token();
    rule_eols_next();

    rule_func_list_next();
    rule_eols_next();

    assert_token_is(EOF_TOKEN);
}

void rule_prolog()
{
    assert_keyword_is(PACKAGE_KEYWORD);
    get_next_token();

    assert_token_is(ID_TOKEN);
    assert_true(strCmpConstStr(current_token.str, "main") == 0, SYNTAX_ERR);
}

void rule_eols_next()
{
    while (current_token.type == EOL_TOKEN)
    {
        get_next_token();
    }
}

void rule_func_list_next()
{
    if (keyword_is(FUNC_KEYWORD))
    {
        rule_func_decl();

        get_next_token();
        rule_eols_next();

        rule_func_list_next();
    }
}

void rule_func_decl()
{
    assert_keyword_is(FUNC_KEYWORD);

    get_next_token();
    assert_token_is(ID_TOKEN);

    string func_name;
    strInit(&func_name);
    strCopyString(&func_name, current_token.str);

    get_next_token();
    assert_token_is(LEFT_BRACKET_TOKEN);

    get_next_token();
    DLInsertLast(&scoped_symtables);
    symtable_init(&scoped_symtables.Last->root_ptr);
    typeQueueInit(&typeQ);

    rule_param_first_next(&scoped_symtables.Last->root_ptr);
    assert_token_is(RIGHT_BRACKET_TOKEN);
    varType* paramArr = typeQueueToArray(&typeQ);
    int paramArrLength = typeQueueLength(&typeQ);

    typeQueueInit(&typeQ);
    get_next_token();
    rule_return_list_next();
    varType* returnArr = typeQueueToArray(&typeQ);
    int returnArrLength = typeQueueLength(&typeQ);


    def_func(&func_name, paramArr, paramArrLength, returnArr, returnArrLength, true);
    strClear(&func_name);
    strAddChar(&func_name, '_');
    def_var(&func_name, UNDERSCORE);
    functionHasReturn = false;
    rule_body();
    if (!functionHasReturn && ((symbol_function*)current_function->data)->return_types_count > 0)
    {
        handle_error(ARGS_RETURNS_COUNT_ERR);
    }
    DLDeleteLast(&scoped_symtables);
}

void rule_param_first_next(symbol_node** function_node)
{
    if (current_token.type == RIGHT_BRACKET_TOKEN)
    {
        return;
    }
    else if (current_token.type == ID_TOKEN)
    {
        rule_param(function_node);
        get_next_token();
        rule_param_n_next(function_node);
    }
    else
    {
        handle_error(SYNTAX_ERR);
    }
}

void rule_param(symbol_node** function_root)
{
    if (current_token.type != ID_TOKEN)
    {
        handle_error(SYNTAX_ERR);
    }
    string param_name;
    strInit(&param_name);
    strCopyString(&param_name, current_token.str);

    get_next_token();
    varType type = rule_type();
    symtable_insert_node_var(function_root, &param_name, type);
    typeQueueUp(&typeQ, type);
}

void rule_param_n_next(symbol_node** function_root)
{
    if (current_token.type == COMMA_TOKEN)
    {
        get_next_token();
        rule_param(function_root);
        get_next_token();
        rule_param_n_next(function_root);
    }
    else if (current_token.type == RIGHT_BRACKET_TOKEN)
    {
        return;
    }
}

void rule_return_list_next()
{
    if (current_token.type == LEFT_BRACKET_TOKEN)
    {
        get_next_token();
        rule_type_first_next();
        assert_token_is(RIGHT_BRACKET_TOKEN);
        get_next_token();
    }
}

void rule_type_first_next()
{
    if (current_token.type == RIGHT_BRACKET_TOKEN)
    {
        return;
    }
    else if (current_token.type == KEYWORD_TOKEN &&
        (current_token.keyword == INT_KEYWORD ||
            current_token.keyword == FLOAT64_KEYWORD ||
            current_token.keyword == STRING_KEYWORD))
    {
        varType type = rule_type();
        typeQueueUp(&typeQ, type);
        get_next_token();
        rule_type_n_next();
    }
}

void rule_type_n_next()
{
    if (current_token.type == RIGHT_BRACKET_TOKEN)
    {
        return;
    }
    else if (current_token.type == COMMA_TOKEN)
    {
        get_next_token();
        varType type = rule_type();
        typeQueueUp(&typeQ, type);
        get_next_token();
        rule_type_n_next();
    }
    else
    {
        handle_error(SYNTAX_ERR);
    }
}

varType rule_type()
{
    if (current_token.type == KEYWORD_TOKEN)
    {
        if (current_token.keyword == INT_KEYWORD)
            return INT;
        else if (current_token.keyword == STRING_KEYWORD)
            return STRING;
        else if (current_token.keyword == FLOAT64_KEYWORD)
            return FLOAT;
    }
    handle_error(SYNTAX_ERR);
    return 0;
}

void rule_body()
{
    assert_token_is(CURLY_BRACKET_LEFT_TOKEN);
    get_next_token();
    rule_eols_next();
    rule_statement_list_next();
    rule_eols_next();
    assert_token_is(CURLY_BRACKET_RIGHT_TOKEN);
}

void rule_statement_list_next()
{
    if (current_token.type == EOL_TOKEN ||
        current_token.type == CURLY_BRACKET_RIGHT_TOKEN)
    {
        return;
    }
    else
    {
        rule_statement_next();
        assert_token_is(EOL_TOKEN);
        rule_eols_next();
        rule_statement_list_next();
    }
}

void rule_statement_next()
{
    leftSideLength = 0;
    rightSideLength = 0;
    tokenQueueInit(&exprTokenQ);
    if (current_token.type == ID_TOKEN)
    {
        leftSideLength++;
        tokenQueueInit(&tokenQ);
        tokenQueueUp(&tokenQ, current_token);
        get_next_token();
        rule_id_list_next();

        rule_statement_action_next();
    }
    else if (current_token.type == LEFT_BRACKET_TOKEN ||
        current_token.type == INTEGER_LITERAL_TOKEN ||
        current_token.type == STRING_LITERAL_TOKEN ||
        current_token.type == DECIMAL_LITERAL_TOKEN)
    {
        rule_literal_expr_next();
    }
    else if (keyword_is(IF_KEYWORD))
    {
        tokenQueueInit(&exprTokenQ);
        get_next_token();
        rule_expr_next();
        int tokenCount = tokenQueueLength(&exprTokenQ);
        token* tokenArr = tokenQueueToArray(&exprTokenQ);
        varType type;
        int result = parse_expression(&scoped_symtables, tokenArr, tokenCount, &type);
        assert_true(result == 0, result);
        assert_true(type == BOOL, DATATYPE_COMPATIBILITY_ERR);

        DLInsertLast(&scoped_symtables);
        symtable_init(&scoped_symtables.Last->root_ptr);
        rule_body();
        DLDeleteLast(&scoped_symtables);

        get_next_token();
        assert_keyword_is(ELSE_KEYWORD);

        get_next_token();
        rule_eols_next();

        DLInsertLast(&scoped_symtables);
        symtable_init(&scoped_symtables.Last->root_ptr);
        rule_body();
        DLDeleteLast(&scoped_symtables);

        get_next_token();
    }
    else if (keyword_is(FOR_KEYWORD))
    {
        DLInsertLast(&scoped_symtables);
        symtable_init(&scoped_symtables.Last->root_ptr);
        get_next_token();
        rule_definition_next();
        assert_token_is(SEMICOLON_TOKEN);

        get_next_token();
        tokenQueueInit(&exprTokenQ);
        rule_expr_next();
        assert_token_is(SEMICOLON_TOKEN);

        int tokenCount = tokenQueueLength(&exprTokenQ);
        token* tokenArr = tokenQueueToArray(&exprTokenQ);
        varType type;
        int result = parse_expression(&scoped_symtables, tokenArr, tokenCount, &type);
        assert_true(result == 0, result);
        assert_true(type == BOOL, DATATYPE_COMPATIBILITY_ERR);

        get_next_token();
        rule_assignment_next();

        rule_body();
        DLDeleteLast(&scoped_symtables);

        get_next_token();
    }
    else if (keyword_is(RETURN_KEYWORD))
    {

        functionHasReturn = true;
        tokenQueueInit(&exprTokenQ);
        typeQueueInit(&typeQ);

        get_next_token();
        rule_statement_value_next();
        assert_true(rightSideLength == ((symbol_function*)current_function->data)->return_types_count, ARGS_RETURNS_COUNT_ERR);
        varType* returnTypeArr = typeQueueToArray(&typeQ);
        assert_true(types_equal(returnTypeArr,
            ((symbol_function*)current_function->data)->return_types,
            rightSideLength), ARGS_RETURNS_COUNT_ERR);

    }
    else
    {
        handle_error(SYNTAX_ERR);
    }
}

void rule_id_list_next()
{
    if (current_token.type == COMMA_TOKEN)
    {
        leftSideLength++;
        get_next_token();
        assert_token_is(ID_TOKEN);
        tokenQueueUp(&tokenQ, current_token);
        get_next_token();
        rule_id_list_next();
        assert_true(token_is(ASSIGNMENT_TOKEN), SYNTAX_ERR);
    }
}

void rule_statement_action_next()
{
    if (current_token.type == LEFT_BRACKET_TOKEN) //function call with no returns
    {
        token function_token;
        tokenQueueGet(&tokenQ, &function_token);
        get_next_token();
        rule_first_arg_next();
        assert_token_is(RIGHT_BRACKET_TOKEN);
        int paramLength = tokenQueueLength(&tokenQ);
        if (paramLength > 0)
        {
            token* tokenArr = tokenQueueToArray(&tokenQ);
            varType* paramArr = tokenArr_to_varTypeArr(tokenArr, paramLength);
            def_func(function_token.str, paramArr, paramLength, NULL, 0, false);
        }
        else
        {
            def_func(function_token.str, NULL, 0, NULL, 0, false);
        }
        get_next_token();
    }
    else if (current_token.type == ASSIGNMENT_TOKEN)
    {
        typeQueueInit(&typeQ);
        leftTokenArr = tokenQueueToArray(&tokenQ);
        leftSideLength = tokenQueueLength(&tokenQ);
        varType* leftTypeArr = malloc(leftSideLength * sizeof(varType));

        for (size_t i = 0; i < leftSideLength; i++)
        {
            assert_true(get_varType_from_symtable(&scoped_symtables, leftTokenArr[i].str, &leftTypeArr[i]) != 1, VAR_DEFINITION_ERR);
        }

        get_next_token();
        rule_statement_value_next();
        assert_true(leftSideLength == rightSideLength, OTHER_SEMANTIC_ERR);
        varType* righTypeArr = typeQueueToArray(&typeQ);
        assert_true(types_equal(leftTypeArr, righTypeArr, leftSideLength), OTHER_SEMANTIC_ERR);
    }
    else if (current_token.type == SHORT_VAR_DECLARATION_TOKEN)
    {
        token varToken;
        tokenQueueGet(&tokenQ, &varToken);
        if (strCmpConstStr(varToken.str, "_") == 0)
        {
            handle_error(VAR_DEFINITION_ERR);
        }

        // leftTokenArr = tokenQueueToArray(&tokenQ);
        // leftSideLength = tokenQueueLength(&tokenQ);
        get_next_token();
        rightSideLength = 1;
        rule_expr_next();
        int tokenCount = tokenQueueLength(&exprTokenQ);
        token* tokenArr = tokenQueueToArray(&exprTokenQ);
        varType type;
        int result = parse_expression(&scoped_symtables, tokenArr, tokenCount, &type);
        assert_true(result == 0, result);
        def_var(varToken.str, type);
    }
}

void rule_statement_value_next()
{
    rightSideLength = 0;
    if (current_token.type == ID_TOKEN)
    {
        tokenQueueUp(&exprTokenQ, current_token);
        rightSideLength++;
        string* tokenName = get_token_str(&current_token);
        get_next_token();
        rule_arg_expr_next(tokenName);
    }
    else if (
        current_token.type == DECIMAL_LITERAL_TOKEN ||
        current_token.type == INTEGER_LITERAL_TOKEN ||
        current_token.type == STRING_LITERAL_TOKEN)
    {
        tokenQueueUp(&exprTokenQ, current_token);
        rightSideLength++;
        rule_literal_expr_next();
        rule_expr_n_next();
    }
}

void rule_arg_expr_next(string* prevTokenName)
{
    if (current_token.type == LEFT_BRACKET_TOKEN) //function call with assignment
    {
        get_next_token();
        tokenQueueInit(&tokenQ);
        rule_first_arg_next();
        assert_token_is(RIGHT_BRACKET_TOKEN);

        int paramArrCount = tokenQueueLength(&tokenQ);
        token* tokenArr = tokenQueueToArray(&tokenQ);
        varType* paramArr = tokenArr_to_varTypeArr(tokenArr, paramArrCount);

        varType* returnArr = tokenArr_to_varTypeArr(leftTokenArr, leftSideLength);
        for (size_t i = 0; i < leftSideLength; i++)
        {
            typeQueueUp(&typeQ, returnArr[i]);
        }

        def_func(prevTokenName, paramArr, paramArrCount, returnArr, leftSideLength, false);
        rightSideLength = leftSideLength;
        get_next_token();
    }
    else
    {
        if (current_token.type == ID_TOKEN)
        {
            assert_true(check_var_defined(prevTokenName), VAR_DEFINITION_ERR);
        }

        rule_expr_end_next();
        rule_expr_n_next();
    }
}

void rule_expr_n_next()
{
    int tokenCount = tokenQueueLength(&exprTokenQ);
    token* tokenArr = tokenQueueToArray(&exprTokenQ);
    varType type;
    int result = parse_expression(&scoped_symtables, tokenArr, tokenCount, &type);
    assert_true(result == 0, result);
    typeQueueUp(&typeQ, type);
    tokenQueueInit(&exprTokenQ);

    if (current_token.type == COMMA_TOKEN)
    {
        rightSideLength++;
        get_next_token();
        rule_expr_next();
        rule_expr_n_next();
    }
    assert_token_is(EOL_TOKEN);
}

void rule_first_arg_next()
{
    if (token_is(RIGHT_BRACKET_TOKEN))
    {
        return;
    }
    rule_term();
    tokenQueueUp(&tokenQ, current_token);

    get_next_token();
    rule_arg_n_next();
}

void rule_arg_n_next()
{
    if (current_token.type == COMMA_TOKEN)
    {
        get_next_token();
        rule_term();
        tokenQueueUp(&tokenQ, current_token);

        get_next_token();
        rule_arg_n_next();
    }
}

void rule_definition_next()
{
    if (current_token.type == ID_TOKEN)
    {
        string var_name;
        strInit(&var_name);
        strCopyString(&var_name, current_token.str);
        get_next_token();
        assert_token_is(SHORT_VAR_DECLARATION_TOKEN);

        get_next_token();
        tokenQueueInit(&exprTokenQ);
        rule_expr_next();

        int tokenCount = tokenQueueLength(&exprTokenQ);
        token* tokenArr = tokenQueueToArray(&exprTokenQ);
        varType type;
        int result = parse_expression(&scoped_symtables, tokenArr, tokenCount, &type);
        assert_true(result == 0, result);
        def_var(&var_name, type);
        strFree(&var_name);
    }
}

void rule_assignment_next()
{
    if (current_token.type == ID_TOKEN)
    {
        assert_true(check_var_defined(current_token.str), VAR_DEFINITION_ERR);
        string var_name;
        strInit(&var_name);
        strCopyString(&var_name, current_token.str);

        get_next_token();
        assert_token_is(ASSIGNMENT_TOKEN);

        tokenQueueInit(&exprTokenQ);

        get_next_token();
        rule_expr_next();

        int tokenCount = tokenQueueLength(&exprTokenQ);
        token* tokenArr = tokenQueueToArray(&exprTokenQ);
        varType expr_type;
        int result = parse_expression(&scoped_symtables, tokenArr, tokenCount, &expr_type);
        assert_true(result == 0, result);

        varType var_type;
        get_varType_from_symtable(&scoped_symtables, &var_name, &var_type);
        assert_true(var_type == expr_type, OTHER_SEMANTIC_ERR);
    }
}

void rule_literal_expr_next()
{
    if (current_token.type == LEFT_BRACKET_TOKEN)
    {
        get_next_token();
        rule_expr_next();
        assert_token_is(RIGHT_BRACKET_TOKEN);

        get_next_token();
        rule_expr_end_next();
    }
    else
    {
        rule_literal();

        get_next_token();
        rule_expr_end_next();
    }
}

void rule_expr_next()
{
    tokenQueueUp(&exprTokenQ, current_token);
    if (current_token.type == ID_TOKEN ||
        current_token.type == DECIMAL_LITERAL_TOKEN ||
        current_token.type == INTEGER_LITERAL_TOKEN ||
        current_token.type == STRING_LITERAL_TOKEN)
    {
        if (current_token.type == ID_TOKEN)
        {
            assert_true(check_var_defined(current_token.str), VAR_DEFINITION_ERR);
            assert_true(strCmpConstStr(current_token.str, "_") != 0, OTHER_SEMANTIC_ERR);
        }

        // TODO: GENERATE_CODE(save stack value to var)
        // needs to save the value that is at the top of the stack to the output var (a := expr)
        // use POPS frame@var

        rule_term();

        get_next_token();
        rule_expr_end_next();
    }
    else if (current_token.type == LEFT_BRACKET_TOKEN)
    {
        get_next_token();
        rule_expr_next();

        assert_token_is(RIGHT_BRACKET_TOKEN);
        tokenQueueUp(&exprTokenQ, current_token);

        get_next_token();
        rule_expr_end_next();
    }
    else
    {
        handle_error(SYNTAX_ERR);
    }
}

void rule_term()
{
    if (current_token.type == ID_TOKEN)
    {
        assert_true(check_var_defined(current_token.str), VAR_DEFINITION_ERR);
        assert_true(strCmpConstStr(current_token.str, "_") != 0, OTHER_SEMANTIC_ERR);
        return;
    }
    else
    {
        rule_literal();
    }
}

void rule_literal()
{
    if (current_token.type == STRING_LITERAL_TOKEN ||
        current_token.type == DECIMAL_LITERAL_TOKEN ||
        current_token.type == INTEGER_LITERAL_TOKEN)
    {
        return;
    }
    else
    {
        handle_error(SYNTAX_ERR);
    }
}

void rule_expr_end_next()
{
    if (current_token.type == PLUS_TOKEN ||
        current_token.type == MINUS_TOKEN ||
        current_token.type == MULTIPLICATION_TOKEN ||
        current_token.type == DIVISON_TOKEN ||
        current_token.type == LESS_TOKEN ||
        current_token.type == GREATER_TOKEN ||
        current_token.type == LESS_EQUAL_TOKEN ||
        current_token.type == GREATER_EQUAL_TOKEN ||
        current_token.type == EQUALS_TOKEN ||
        current_token.type == NOT_EQUALS_TOKEN)
    {
        tokenQueueUp(&exprTokenQ, current_token);
        get_next_token();
        rule_expr_next();
    }
}

//----------HELPERS----------

void func_handle_error(int errType, char const* func)
{
    char* token = strGetStr(current_token.token_str_raw);

    switch (errType)
    {
        case SYNTAX_ERR:
            fprintf(stderr, "[Call from '%s']. Syntax error. Unexpected token '%s' of type %d on line %d\n",
                func, token, current_token.type, current_token.source_line);
            exit(SYNTAX_ERR);
            break;
        case LEX_ERR:
            fprintf(stderr, "Lexical error on line %d\n",
                current_token.source_line);
            exit(LEX_ERR);

        default:
            fprintf(stderr, "[Call from '%s']. Error no %d. On line %d\n",
                func, errType, current_token.source_line);
            exit(errType);
    }
}

void get_next_token()
{
    if (get_token(&current_token) == LEX_ERR)
    {
        handle_error(LEX_ERR);
    }
}

bool token_is(token_type token)
{
    return current_token.type == token;
}

void func_assert_token_is(token_type token, char const* func)
{
    if (current_token.type != token)
    {
        func_handle_error(SYNTAX_ERR, func);
    }
}

void func_assert_true(bool expression, int errno, char const* func)
{
    if (!expression)
    {
        func_handle_error(errno, func);
    }
}

bool keyword_is(keyword keyword)
{
    return current_token.type == KEYWORD_TOKEN && current_token.keyword == keyword;
}

void func_assert_keyword_is(keyword keyword, char const* func)
{
    if (!keyword_is(keyword))
    {
        func_handle_error(SYNTAX_ERR, func);
    }
}

char* copystr(string* str)
{
    char* charArr = malloc(str->length);
    strcpy(charArr, str->str);
    return charArr;
}

bool types_equal(varType* types1, varType* types2, int length)
{

    for (size_t i = 0; i < length; i++)
    {
        if (types1[i] == UNDERSCORE || types2[i] == UNDERSCORE)
            continue;
        if (types1[i] != types2[i])
        {
            return false;
        }
    }
    return true;
}

void def_func(string* func_name, varType* paramArr, int paramArrLength, varType* returnArr, int returnArrLength, bool definition)
{
    symbol_node* found_func;
    if (symtable_search(&functions_symtable, func_name, &found_func)) //func already exists
    {
        symbol_function* data = (symbol_function*)(found_func->data);
        if (definition && data->defined) //redefinition
        {
            handle_error(VAR_DEFINITION_ERR);
        }
        if (data->par_count != -1)// if par_count is -1, then params can be arbitrary
        {
            assert_true(data->par_count == paramArrLength, ARGS_RETURNS_COUNT_ERR);
            assert_true(types_equal(paramArr, data->parameters, paramArrLength), ARGS_RETURNS_COUNT_ERR);
        }
        assert_true(data->return_types_count == returnArrLength, ARGS_RETURNS_COUNT_ERR);
        assert_true(types_equal(returnArr, data->return_types, returnArrLength), ARGS_RETURNS_COUNT_ERR);
        if (!data->defined)
        {
            data->defined = definition;
        }
    }
    else //insert function into symtable
    {
        symtable_insert_node_func(&functions_symtable, func_name, returnArrLength, returnArr, paramArrLength, paramArr, definition);
    }
    if (definition)
    {
        assert_true(symtable_search(&functions_symtable, func_name, &current_function), INTERNAL_COMPILER_ERR);
    }
}

void def_var(string* var_name, varType type)
{
    table* current_symtable = scoped_symtables.Last;
    varType existing_varType;
    int declaredIndex = get_varType_from_symtable(&scoped_symtables, var_name, &existing_varType);
    if (declaredIndex == -1 || declaredIndex < current_symtable->scope_index) //if var doesnt exist or is defied at lesser scope
    {
        symtable_insert_node_var(&current_symtable->root_ptr, var_name, type);
    }
    else //redefinition
    {
        handle_error(VAR_DEFINITION_ERR);
    }
}

varType* tokenArr_to_varTypeArr(token* tokenArr, int count)
{
    if (count == 0)
    {
        return NULL;
    }
    varType* typeArr = malloc(count * sizeof(tokenArr));
    for (size_t i = 0; i < count; i++)
    {
        if (tokenArr[i].type == ID_TOKEN)
        {
            if (get_varType_from_symtable(&scoped_symtables, tokenArr[i].str, &typeArr[i]) == -1)
            {
                handle_error(VAR_DEFINITION_ERR);
            }
        }
        else
        {
            typeArr[i] = get_varType_from_literal(tokenArr[i].type);
        }
    }
    return typeArr;
}

//searches scopes backwards and returns the scope of the first occurence of input name and returns its type
int get_varType_from_symtable(tDLList* scoped_symtables, string* varName, varType* type)
{
    table* symtable = scoped_symtables->Last;
    symbol_node* foundNode;
    while (symtable != NULL)
    {
        symbol_node* rootNode = symtable->root_ptr;
        if (rootNode != NULL && symtable_search(&rootNode, varName, &foundNode))
        {
            *type = ((symbol_variable*)foundNode->data)->var_type;
            return symtable->scope_index;
        }
        symtable = symtable->prev_table;
    }
    return -1;
}

varType get_varType_from_literal(token_type type)
{
    switch (type)
    {
        case DECIMAL_LITERAL_TOKEN:
            return FLOAT;
            break;
        case INTEGER_LITERAL_TOKEN:
            return INT;
            break;
        case STRING_LITERAL_TOKEN:
            return STRING;
            break;

        default:
            handle_error(SYNTAX_ERR);
            return UNDEFINED;
            break;
    }
}

bool check_var_defined(string* varName)
{
    varType _;
    int res = get_varType_from_symtable(&scoped_symtables, varName, &_);
    if (res == -1)
        return false;
    else
        return true;
}

string* get_token_str(token* token)
{
    string* newStr = malloc(sizeof(string));
    strInit(newStr);
    strCopyString(newStr, token->str);
    return newStr;
}
