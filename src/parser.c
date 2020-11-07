#include "headers/parser.h"
#include "headers/parser_rules.h"
#include "headers/parser_helpers.h"
#include "headers/scanner.h"
#include "headers/symtable.h"
#include "headers/error_codes.h"
#include <stdio.h>
#include <stdbool.h>

token current_token;

void parser_start()
{
    string str;
    strInit(&str);
    current_token.str = &str;

    rule_prog();
    exit(0);
}

void rule_prog()
{
    get_next_token();
    rule_eols();

    rule_prolog();
    get_next_token();
    if (current_token.type != EOL_TOKEN)
    {
        handle_error(SYNTAX_ERR);
    }
    get_next_token();

    rule_eols();


    rule_func_decl();

    get_next_token();
    rule_eols();

    rule_func_list();

    assert_token_is(EOF_TOKEN);
}

void rule_prolog()
{
    assert_keyword_is(PACKAGE_KEYWORD);

    get_next_token();
    assert_token_is(ID_TOKEN);
}

void rule_eols()
{
    while (current_token.type == EOL_TOKEN)
    {
        get_next_token();
    }
}

void rule_func_list()
{
    if (keyword_is(FUNC_KEYWORD))
    {
        rule_func_decl();

        get_next_token();
        rule_eols();

        rule_func_list();
    }
}

void rule_func_decl()
{
    if (current_token.type != KEYWORD_TOKEN || current_token.keyword != FUNC_KEYWORD)
    {
        handle_error(SYNTAX_ERR);
    }
    get_next_token();
    if (current_token.type != ID_TOKEN)
    {
        handle_error(SYNTAX_ERR);
    }
    get_next_token();
    if (current_token.type != LEFT_BRACKET_TOKEN)
    {
        handle_error(SYNTAX_ERR);
    }
    get_next_token();
    rule_param_first();
    if (current_token.type != RIGHT_BRACKET_TOKEN)
    {
        handle_error(SYNTAX_ERR);
    }
    get_next_token();
    rule_return_list();


    rule_body();
}

void rule_param_first()
{
    if (current_token.type == RIGHT_BRACKET_TOKEN)
    {
        return;
    }
    else if (current_token.type == ID_TOKEN)
    {
        rule_param();
        get_next_token();
        rule_param_n();

    }
    else
    {
        handle_error(SYNTAX_ERR);
    }
}

void rule_param()
{
    if (current_token.type != ID_TOKEN)
    {
        handle_error(SYNTAX_ERR);
    }
    get_next_token();
    rule_type();
}

void rule_param_n()
{
    if (current_token.type == COMMA_TOKEN)
    {
        get_next_token();
        rule_param();
        get_next_token();
        rule_param_n();
    }
    else if (current_token.type == RIGHT_BRACKET_TOKEN)
    {
        return;
    }
}

void rule_return_list()
{
    if (current_token.type == LEFT_BRACKET_TOKEN)
    {
        get_next_token();
        rule_type_first();
        assert_token_is(RIGHT_BRACKET_TOKEN);
        get_next_token();
    }
}

void rule_type_first()
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
        rule_type();
        get_next_token();
        rule_type_n();
    }
}

void rule_type_n()
{
    if (current_token.type == RIGHT_BRACKET_TOKEN)
    {
        return;
    }
    else if (current_token.type == COMMA_TOKEN)
    {
        get_next_token();
        rule_type();
        get_next_token();
        rule_type_n();
    }
    else
    {
        handle_error(SYNTAX_ERR);
    }
}

void rule_type()
{
    if (current_token.type == KEYWORD_TOKEN)
    {
        if (current_token.keyword == INT_KEYWORD ||
            current_token.keyword == STRING_KEYWORD ||
            current_token.keyword == FLOAT64_KEYWORD)
        {
            return;
        }
    }
    handle_error(SYNTAX_ERR);

}

void rule_body()
{
    assert_token_is(CURLY_BRACKET_LEFT_TOKEN);
    get_next_token();
    rule_eols();
    rule_statement_list();
    rule_eols();
    assert_token_is(CURLY_BRACKET_RIGHT_TOKEN);
}

void rule_statement_list()
{
    if (current_token.type == EOL_TOKEN ||
        current_token.type == CURLY_BRACKET_RIGHT_TOKEN)
    {
        return;
    }
    else
    {
        rule_statement();
        assert_token_is(EOL_TOKEN);
        rule_eols();
        rule_statement_list();
    }
}

void rule_statement()
{
    if (current_token.type == ID_TOKEN)
    {
        get_next_token();
        rule_id_list();
        rule_statement_action();
    }
    else if (current_token.type == LEFT_BRACKET_TOKEN ||
        current_token.type == INTEGER_LITERAL_TOKEN ||
        current_token.type == STRING_LITERAL_TOKEN ||
        current_token.type == DECIMAL_LITERAL_TOKEN)
    {
        rule_literal_expr();
    }
    else if (keyword_is(IF_KEYWORD))
    {
        get_next_token();
        rule_expr();
        rule_body();

        get_next_token();
        assert_keyword_is(ELSE_KEYWORD);

        get_next_token();
        rule_eols();

        rule_body();
    }
    else if (keyword_is(FOR_KEYWORD))
    {
        get_next_token();
        rule_definition();

        assert_token_is(SEMICOLON_TOKEN);

        get_next_token();
        rule_expr();

        get_next_token();
        assert_token_is(SEMICOLON_TOKEN);

        get_next_token();
        rule_assignment();

        rule_body();
    }
    else if (keyword_is(RETURN_KEYWORD))
    {
        get_next_token();
        rule_statement_value();
    }
    else
    {
        handle_error(SYNTAX_ERR);
    }
}


void rule_id_list()
{
    if (current_token.type == COMMA_TOKEN)
    {
        get_next_token();
        assert_token_is(ID_TOKEN);
        get_next_token();
        rule_id_list();
    }
}

void rule_statement_action()
{
    if (current_token.type == LEFT_BRACKET_TOKEN)
    {
        get_next_token();
        rule_first_arg();
        assert_token_is(RIGHT_BRACKET_TOKEN);
        get_next_token();
    }
    else if (current_token.type == ASSIGNMENT_TOKEN)
    {
        get_next_token();
        rule_statement_value();
    }
    else if (current_token.type == SHORT_VAR_DECLARATION_TOKEN)
    {
        get_next_token();
        rule_expr();
    }
}

void rule_statement_value()
{
    if (current_token.type == ID_TOKEN)
    {
        get_next_token();
        rule_arg_expr();
    }
    else if (
        current_token.type == DECIMAL_LITERAL_TOKEN ||
        current_token.type == INTEGER_LITERAL_TOKEN ||
        current_token.type == STRING_LITERAL_TOKEN)
    {
        rule_literal_expr();
        rule_expr_n();
    }
}

void rule_arg_expr()
{
    if (current_token.type == LEFT_BRACKET_TOKEN)
    {
        get_next_token();
        rule_first_arg();
        assert_token_is(RIGHT_BRACKET_TOKEN);
        get_next_token();

    }
    else
    {
        rule_expr_end();
        rule_expr_n();
    }
}

void rule_expr_n()
{
    if (current_token.type == COMMA_TOKEN)
    {
        get_next_token();
        rule_expr();
    }
    assert_token_is(EOL_TOKEN);
}

void rule_first_arg()
{
    if (token_is(RIGHT_BRACKET_TOKEN))
    {
        return;
    }
    rule_term();

    get_next_token();
    rule_arg_n();
}

void rule_arg_n()
{
    if (current_token.type == COMMA_TOKEN)
    {
        get_next_token();
        rule_term();

        get_next_token();
        rule_arg_n();
    }
}

void rule_definition()
{
    if (current_token.type == ID_TOKEN)
    {
        get_next_token();
        assert_token_is(SHORT_VAR_DECLARATION_TOKEN);

        get_next_token();
        rule_expr();
    }
}

void rule_assignment()
{
    if (current_token.type == ID_TOKEN)
    {
        get_next_token();
        assert_token_is(ASSIGNMENT_TOKEN);

        get_next_token();
        rule_expr();
    }
}

void rule_literal_expr()
{
    if (current_token.type == LEFT_BRACKET_TOKEN)
    {
        get_next_token();
        rule_expr();
        assert_token_is(RIGHT_BRACKET_TOKEN);

        get_next_token();
        rule_expr_end();
    }
    else
    {
        rule_literal();

        get_next_token();
        rule_expr_end();
    }
}

void rule_expr()
{
    if (current_token.type == ID_TOKEN ||
        current_token.type == DECIMAL_LITERAL_TOKEN ||
        current_token.type == INTEGER_LITERAL_TOKEN ||
        current_token.type == STRING_LITERAL_TOKEN)
    {
        //TODO CALL EXPRESSION module
        rule_term();

        get_next_token();
        rule_expr_end();
    }
    else if (current_token.type == LEFT_BRACKET_TOKEN)
    {
        get_next_token();
        rule_expr();

        get_next_token();
        assert_token_is(RIGHT_BRACKET_TOKEN);

        get_next_token();
        rule_expr_end();
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

void rule_expr_end()
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
        rule_expr();
    }
}


//----------HELPERS----------

void handle_error(int errType)
{
    switch (errType)
    {
        case SYNTAX_ERR:
            fprintf(stderr, "Syntax error. Unexpected token %d\n", current_token.type);
            exit(SYNTAX_ERR);
            break;

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

void assert_token_is(token_type token)
{
    if (current_token.type != token)
    {
        handle_error(SYNTAX_ERR);
    }
}

bool keyword_is(keyword keyword)
{
    return current_token.type == KEYWORD_TOKEN && current_token.keyword == keyword;
}

void assert_keyword_is(keyword keyword)
{
    if (!keyword_is(keyword))
    {
        handle_error(SYNTAX_ERR);
    }
}



