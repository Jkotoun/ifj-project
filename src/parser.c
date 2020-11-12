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
#include "headers/varType_queue.h"
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
tQueue typeQ;

// node rootNode;
// tDLList list;

void parser_start()
{
    string str, rawStr;
    strInit(&str);
    strInit(&rawStr);

    current_token.str = &str;
    current_token.token_str_raw = &rawStr;

    // functions_symtable = &rootNode;
    // scoped_symtables = &list;

    init(&functions_symtable);
    queueInit(&typeQ);
    // insert_node_func(&functions_symtable, "test", 0, 0, 0, 0, true);

    DLInitList(&scoped_symtables);

    rule_prog();
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

    node* found_func;
    bool found = search(&functions_symtable, &func_name, &found_func);
    symbol_function* data;
    if (found)//func already exists
    {
        data = (symbol_function*)(found_func->data);
        if (data->defined)//redefinition
        {
            handle_error(VAR_DEFINITION_ERR);
        }
    }

    get_next_token();
    assert_token_is(LEFT_BRACKET_TOKEN);

    get_next_token();
    DLInsertLast(&scoped_symtables);
    init(&scoped_symtables.Last->root_ptr);
    queueInit(&typeQ);

    rule_param_first_next(&scoped_symtables.Last->root_ptr);
    assert_token_is(RIGHT_BRACKET_TOKEN);
    varType* paramArr = queueToArray(&typeQ);
    int paramArrLength = queueLength(&typeQ);

    queueInit(&typeQ);
    get_next_token();
    rule_return_list_next();
    varType* returnArr = queueToArray(&typeQ);
    int returnArrLength = queueLength(&typeQ);

    if (found)//function is already called but not defined
    {
        assert_true(data->par_count == paramArrLength, ARGS_RETURNS_COUNT_ERR);
        assert_true(data->return_types_count == returnArrLength, ARGS_RETURNS_COUNT_ERR);
        assert_true(types_equal(paramArr, data->parameters, paramArrLength), ARGS_RETURNS_COUNT_ERR);
        assert_true(types_equal(returnArr, data->return_types, returnArrLength), ARGS_RETURNS_COUNT_ERR);
        data->defined = true;
    }
    else//insert function into symtable
    {
        insert_node_func(&functions_symtable, &func_name, returnArrLength, returnArr, paramArrLength, paramArr, true);
    }
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
    queueUp(&typeQ, type);

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
        queueUp(&typeQ, type);
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
        queueUp(&typeQ, type);
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
        get_next_token();
        rule_id_list_next();
        assert_true(token_is(SHORT_VAR_DECLARATION_TOKEN) || token_is(ASSIGNMENT_TOKEN), SYNTAX_ERR);
    }
}

void rule_statement_action_next()
{
    if (current_token.type == LEFT_BRACKET_TOKEN)
    {
        get_next_token();
        rule_first_arg_next();
        assert_token_is(RIGHT_BRACKET_TOKEN);
        get_next_token();
    }
    else if (current_token.type == ASSIGNMENT_TOKEN)
    {
        get_next_token();
        rule_statement_value_next();
    }
    else if (current_token.type == SHORT_VAR_DECLARATION_TOKEN)
    {
        // list.Last->root_ptr= malloc()
        get_next_token();
        rule_expr_next();
    }
}

void rule_statement_value_next()
{
    if (current_token.type == ID_TOKEN)
    {
        get_next_token();
        rule_arg_expr_next();
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

void rule_arg_expr_next()
{
    if (current_token.type == LEFT_BRACKET_TOKEN)
    {
        get_next_token();
        rule_first_arg_next();
        assert_token_is(RIGHT_BRACKET_TOKEN);
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

    get_next_token();
    rule_arg_n_next();
}

void rule_arg_n_next()
{
    if (current_token.type == COMMA_TOKEN)
    {
        get_next_token();
        rule_term();

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
        //TODO CALL EXPRESSION module
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
            fprintf(stderr, "[Call from '%s']. Syntax error. Unexpected token '%s' on line %d\n",
                func, token, current_token.source_line);
            exit(SYNTAX_ERR);
            break;
        case LEX_ERR:
            fprintf(stderr, "Lexical error on line %d\n",
                current_token.source_line);
            exit(LEX_ERR);

        default:
            break;
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
        func_handle_error(SYNTAX_ERR, func);
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