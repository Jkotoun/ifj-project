/****
 * Implementace překladače imperativního jazyka IFJ20.
 * Jiří Vlasák (xvlasa15)
 * Parser
 * CONVENTIONS:
 * 1: every rule expects its first token to be already loaded in current_token variable.
 * 2: rules that read 1 token into next rule end with suffix "_next".
*/

#include "headers/parser.h"
#include "headers/parser_rules.h"
#include "headers/parser_helpers.h"
#include "headers/scanner.h"
#include "headers/symtable.h"
#include "headers/dl_list.h"
#include "headers/error_codes.h"
#include "headers/queue.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define assert_token_is(token) func_assert_token_is(token, __func__)
#define assert_keyword_is(keyword) func_assert_keyword_is(keyword, __func__)
#define assert_true(expression, errno) func_assert_true(expression,errno, __func__)

#define handle_error(errno) func_handle_error(errno, __func__)

token current_token;

node* functions_symtable;
tDLList scoped_symtables;
typeQueue typeQ;
tokenQueue tokenQ;

token* leftTokenArr;
int leftSideLength;
// node rootNode;
// tDLList list;

void parser_start()
{
    string str, rawStr;
    strInit(&str);
    strInit(&rawStr);

    current_token.str = &str;
    current_token.token_str_raw = &rawStr;

    init(&functions_symtable);
    typeQueueInit(&typeQ);
    //TODO: check if package MAIN is defined
    //TODO: after program ends check for undefined functions

    DLInitList(&scoped_symtables);

    rule_prog();

    node* main_node;
    string main_str;
    strInit(&main_str);
    strAddChar(&main_str, 'm');
    strAddChar(&main_str, 'a');
    strAddChar(&main_str, 'i');
    strAddChar(&main_str, 'n');

    if (!search(&functions_symtable, &main_str, &main_node))
    {
        handle_error(OTHER_SEMANTIC_ERR);
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
    assert_true(strCmpConstStr(current_token.str, "main") == 0, OTHER_SEMANTIC_ERR);
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
    init(&scoped_symtables.Last->root_ptr);
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

    func_check(&func_name, paramArr, paramArrLength, returnArr, returnArrLength, true);
    strClear(&func_name);

    // insert_node_var(&(scoped_symtables.Last->root_ptr), "zadar", INT);
    rule_body();
    DLDeleteLast(&scoped_symtables);
}

void rule_param_first_next(node** function_node)
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

void rule_param(node** function_root)
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
    insert_node_var(function_root, &param_name, type);
    typeQueueUp(&typeQ, type);

}

void rule_param_n_next(node** function_root)
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
    if (current_token.type == ID_TOKEN)
    {
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
        get_next_token();
        rule_expr_next();
        rule_body();

        get_next_token();
        assert_keyword_is(ELSE_KEYWORD);

        get_next_token();
        rule_eols_next();

        rule_body();
        get_next_token();
    }
    else if (keyword_is(FOR_KEYWORD))
    {
        get_next_token();
        rule_definition_next();
        assert_token_is(SEMICOLON_TOKEN);

        get_next_token();
        rule_expr_next();
        assert_token_is(SEMICOLON_TOKEN);

        get_next_token();
        rule_assignment_next();

        rule_body();
        get_next_token();
    }
    else if (keyword_is(RETURN_KEYWORD))
    {
        get_next_token();
        rule_statement_value_next();
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
        get_next_token();
        assert_token_is(ID_TOKEN);
        tokenQueueUp(&tokenQ, current_token);
        get_next_token();
        rule_id_list_next();
        assert_true(token_is(SHORT_VAR_DECLARATION_TOKEN) || token_is(ASSIGNMENT_TOKEN), SYNTAX_ERR);
    }
}

void rule_statement_action_next()
{
    if (current_token.type == LEFT_BRACKET_TOKEN)//function call with no returns
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
            func_check(function_token.str, paramArr, paramLength, NULL, 0, false);
        }
        else
        {
            func_check(function_token.str, NULL, 0, NULL, 0, false);
        }
        get_next_token();
    }
    else if (current_token.type == ASSIGNMENT_TOKEN)
    {
        leftTokenArr = tokenQueueToArray(&tokenQ);
        leftSideLength = tokenQueueLength(&tokenQ);
        get_next_token();
        rule_statement_value_next();
    }
    else if (current_token.type == SHORT_VAR_DECLARATION_TOKEN)
    {
        leftTokenArr = tokenQueueToArray(&tokenQ);
        leftSideLength = tokenQueueLength(&tokenQ);
        // list.Last->root_ptr= malloc()
        get_next_token();
        rule_expr_next();
    }
}

void rule_statement_value_next()
{
    if (current_token.type == ID_TOKEN)
    {
        string* tokenName = get_token_str(&current_token);
        get_next_token();
        rule_arg_expr_next(tokenName);
    }
    else if (
        current_token.type == DECIMAL_LITERAL_TOKEN ||
        current_token.type == INTEGER_LITERAL_TOKEN ||
        current_token.type == STRING_LITERAL_TOKEN)
    {
        rule_literal_expr_next();
        rule_expr_n_next();
    }
}

void rule_arg_expr_next(string* prevTokenName)
{
    if (current_token.type == LEFT_BRACKET_TOKEN)//function call with assignment
    {
        get_next_token();
        tokenQueueInit(&tokenQ);
        rule_first_arg_next();
        assert_token_is(RIGHT_BRACKET_TOKEN);

        int paramArrCount = tokenQueueLength(&tokenQ);
        token* tokenArr = tokenQueueToArray(&tokenQ);
        varType* paramArr = tokenArr_to_varTypeArr(tokenArr, paramArrCount);

        varType* returnArr = tokenArr_to_varTypeArr(leftTokenArr, leftSideLength);
        func_check(prevTokenName, paramArr, paramArrCount, returnArr, leftSideLength, false);
        get_next_token();
    }
    else
    {
        rule_expr_end_next();
        rule_expr_n_next();
    }
}

void rule_expr_n_next()
{
    if (current_token.type == COMMA_TOKEN)
    {
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
        get_next_token();
        assert_token_is(SHORT_VAR_DECLARATION_TOKEN);

        get_next_token();
        rule_expr_next();
    }
}

void rule_assignment_next()
{
    if (current_token.type == ID_TOKEN)
    {
        get_next_token();
        assert_token_is(ASSIGNMENT_TOKEN);

        get_next_token();
        rule_expr_next();
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
    if (current_token.type == ID_TOKEN ||
        current_token.type == DECIMAL_LITERAL_TOKEN ||
        current_token.type == INTEGER_LITERAL_TOKEN ||
        current_token.type == STRING_LITERAL_TOKEN)
    {
        //TODO: CALL EXPRESSION module
        rule_term();

        get_next_token();
        rule_expr_end_next();
    }
    else if (current_token.type == LEFT_BRACKET_TOKEN)
    {
        get_next_token();
        rule_expr_next();

        assert_token_is(RIGHT_BRACKET_TOKEN);

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
        if (types1[i] != types2[i])
        {
            return false;
        }
    }
    return true;
}

void func_check(string* func_name, varType* paramArr, int paramArrLength, varType* returnArr, int returnArrLength, bool definition)
{
    node* found_func;
    if (search(&functions_symtable, func_name, &found_func))//func already exists
    {
        symbol_function* data = (symbol_function*)(found_func->data);
        if (definition && data->defined)//redefinition
        {
            handle_error(VAR_DEFINITION_ERR);
        }
        assert_true(data->par_count == paramArrLength, ARGS_RETURNS_COUNT_ERR);
        assert_true(data->return_types_count == returnArrLength, ARGS_RETURNS_COUNT_ERR);
        assert_true(types_equal(paramArr, data->parameters, paramArrLength), ARGS_RETURNS_COUNT_ERR);
        assert_true(types_equal(returnArr, data->return_types, returnArrLength), ARGS_RETURNS_COUNT_ERR);
        if (!data->defined)
        {
            data->defined = definition;
        }
    }
    else//insert function into symtable
    {
        insert_node_func(&functions_symtable, func_name, returnArrLength, returnArr, paramArrLength, paramArr, definition);
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
            if (!get_varType_from_symtable(tokenArr[i].str, &typeArr[i]))
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

bool get_varType_from_symtable(string* varName, varType* type)
{
    table* symtable = scoped_symtables.Last;
    node* foundNode;
    while (symtable != NULL)
    {
        node* rootNode = symtable->root_ptr;
        if (rootNode != NULL && search(&rootNode, varName, &foundNode))
        {
            *type = ((symbol_variable*)foundNode->data)->var_type;
            return true;
        }
        symtable = symtable->prev_table;
    }
    return false;
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
            break;
    }
}

string* get_token_str(token* token)
{
    string* newStr = malloc(sizeof(string));
    strInit(newStr);
    strCopyString(newStr, token->str);
    return newStr;
}
