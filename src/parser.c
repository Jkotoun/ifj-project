#include "parser.h"
#include "parser_rules.h"
#include "scanner.h"
#include "symtable.h"
#include "error_codes.h"
#include "stdio.h"

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

}


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



