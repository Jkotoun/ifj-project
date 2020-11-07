#include "headers/parser.h"
#include "headers/parser_rules.h"
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
}

void rule_prog()
{
    get_token(&current_token);

    rule_prolog();
    get_token(&current_token);
    if (current_token.type != EOL_TOKEN)
    {
        handle_error(SYNTAX_ERR);
    }
    get_token(&current_token);

    rule_eols();


    rule_func_decl();
}

void rule_prolog()
{

    switch (current_token.type)
    {
        case KEYWORD_TOKEN:
            switch (current_token.keyword)
            {
                case PACKAGE_KEYWORD:
                    /* code */
                    break;

                default:
                    handle_error(SYNTAX_ERR);
                    break;
            }
            break;

        default:

            handle_error(SYNTAX_ERR);
            break;
    }

    get_token(&current_token);

    switch (current_token.type)
    {
        case ID_TOKEN:
            break;

        default:

            handle_error(SYNTAX_ERR);
            break;
    }

}

void rule_eols()
{
    while (current_token.type == EOL_TOKEN)
    {
        get_token(&current_token);
    }
    switch (current_token.type)
    {
        case EOF_TOKEN:
        case ID_TOKEN:
        case LEFT_BRACKET_TOKEN:
        case CURLY_BRACKET_LEFT_TOKEN:
        case CURLY_BRACKET_RIGHT_TOKEN:
        case DECIMAL_LITERAL_TOKEN:
        case STRING_LITERAL_TOKEN:
        case INTEGER_LITERAL_TOKEN:
            break;

        case KEYWORD_TOKEN:
            switch (current_token.keyword)
            {
                case FUNC_KEYWORD:
                case IF_KEYWORD:
                case FOR_KEYWORD:
                case RETURN_KEYWORD:
                    break;

                default:
                    handle_error(SYNTAX_ERR);
                    break;
            }
            break;

        default:
            handle_error(SYNTAX_ERR);
            break;
    }

}

void rule_func_decl()
{
    if (current_token.type != KEYWORD_TOKEN || current_token.keyword != FUNC_KEYWORD)
    {
        handle_error(SYNTAX_ERR);
    }
    get_token(&current_token);
    if (current_token.type != ID_TOKEN)
    {
        handle_error(SYNTAX_ERR);
    }
    get_token(&current_token);
    if (current_token.type != LEFT_BRACKET_TOKEN)
    {
        handle_error(SYNTAX_ERR);
    }
    get_token(&current_token);
    rule_param_first();
    if (current_token.type != RIGHT_BRACKET_TOKEN)
    {
        handle_error(SYNTAX_ERR);
    }
    get_token(&current_token);
    rule_return_list();
    get_next_token();
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
        get_token(&current_token);
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
    get_token(&current_token);
    rule_type();
}

void rule_param_n()
{
    if (current_token.type == COMMA_TOKEN)
    {
        get_token(&current_token);
        rule_param();
        get_token(&current_token);
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
        get_token(&current_token);
        rule_type_first();
        assert_token_is(RIGHT_BRACKET_TOKEN);
    }
}

void rule_type_first()
{
    get_next_token();
    if (current_token.type == RIGHT_BRACKET_TOKEN)
    {
        return;
    }
    else if (current_token.type == KEYWORD_TOKEN &&
        current_token.keyword == INT_KEYWORD ||
        current_token.keyword == FLOAT64_KEYWORD ||
        current_token.keyword == STRING_KEYWORD)
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
        current_token.type == RIGHT_BRACKET_TOKEN)
    {
        return;
    }
    else
    {
        rule_statement();

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
    else if (keyword_equals(IF_KEYWORD))
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
    else if (keyword_equals(FOR_KEYWORD))
    {
        get_next_token();
        rule_definition();

        get_next_token();
        assert_token_is(SEMICOLON_TOKEN);

        get_next_token();
        rule_expr();

        get_next_token();
        assert_token_is(SEMICOLON_TOKEN);

        get_next_token();
        rule_assignment();

        rule_body();
    }
    else if (keyword_equals(RETURN_KEYWORD))
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



//----------HELPERS----------

void handle_error(int errType)
{
    switch (errType)
    {
        case SYNTAX_ERR:
            fprintf(stderr, "Syntax error.\n");
            exit(SYNTAX_ERR);
            break;

        default:
            break;
    }
}

void assert_token_is(token_type token)
{
    if (current_token.type != token)
    {
        handle_error(SYNTAX_ERR);
    }
}

void get_next_token()
{
    get_token(&current_token);
}

bool keyword_equals(keyword keyword)
{
    return current_token.type == KEYWORD_TOKEN && current_token.keyword == keyword;
}

void assert_keyword_is(keyword keyword)
{
    if (!keyword_equals(keyword))
    {
        handle_error(SYNTAX_ERR);
    }
}



