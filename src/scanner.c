/****
 * Implementace překladače imperativního jazyka IFJ20.
 * Josef Kotoun - xkotou06
 * Lexical analysis (scanner)
*/
#include <stdio.h>
#include <ctype.h>
#include "str.h"
#include "scanner.h"
//#include "error_codes.h"

#define OK 1
#define LEX_ERR 0
#define INTERNAL_COMPILER_ERR 2
static keyword_str_pair all_keywords_pairs[] = 
{
    {INT_KEYWORD, "int"},
    {ELSE_KEYWORD, "else"},
    {FLOAT64_KEYWORD, "float64"},
    {FOR_KEYWORD, "for"},
    {FUNC_KEYWORD, "func"},
    {IF_KEYWORD, "func"},
    {PACKAGE_KEYWORD, "package"},
    {RETURN_KEYWORD, "return"},
    {STRING_KEYWORD, "string"}
};
FILE *source_file;
//iterates over keywords pairs strings and if its same, returns true and passes enum code of keyword to parameter *keyword
bool str_is_keyword(string *str, keyword *keyword)
{
    for(int i = 0;i<(sizeof(all_keywords_pairs) / sizeof(keyword_str_pair));i++)
    {
        //string is same as some of keyword string representation
        if(!strCmpConstStr(str,all_keywords_pairs[i].keyword_str))
        {
            *keyword = all_keywords_pairs[i].keyword;
            return true;
        }
    }
    return false;
}
void setSourceFile(FILE* file)
{
    source_file = file;
}

int get_token(token *token)
{
    int c;
    strClear(token->str);
    if(token == NULL)
    {
        return INTERNAL_COMPILER_ERR;
    }
    state scanner_state = INITIAL_STATE;
    while(1)
    {   
        c = getc(source_file);
        switch(scanner_state)
        {
            case INITIAL_STATE:
                if(isspace(c))
                {
                    scanner_state = INITIAL_STATE;
                }
                else if (c == ';') 
                { 
                    token->type = SEMICOLON_TOKEN;
                    return OK; 
                }
                else if (c == ',') 
                { 
                    token->type = COMMA_TOKEN;
                    return OK; 
                }
                else if (c == '+') 
                { 
                    token->type = PLUS_TOKEN; 
                    return OK;
                }
                else if (c == '=') 
                { 
                    scanner_state = EQUALS_ONCE_STATE;
                    return OK;
                }
                else if (c == '-') 
                { 
                    token->type = MINUS_TOKEN; 
                    return OK;
                }
                else if (c == '*') 
                { 
                    token->type = MULTIPLICATION_TOKEN; 
                    return OK;
                }
                else if (c == ':')
                {
                    scanner_state = COLON_STATE;
                }
                else if (c == '!')
                {
                    scanner_state = EXCLAMATION_MARK_STATE;
                }
                else if(c == '<')
                {
                    scanner_state == LESS_STATE;
                }
                else if(c == '>')
                {
                    scanner_state = GREATER_STATE;
                }
                else if(isalpha(c) || c == '_')
                {
                    scanner_state = ID_KEYWORD_STATE;
                }
                else if(isdigit(c))
                {
                    scanner_state = NUM_STATE;
                }
                else if(c == '/')
                {
                    scanner_state = SLASH_STATE;
                }
                else if(c == '"')
                {
                    scanner_state = STRING_STATE; 
                }
                else if(c == '\n')
                {
                    token->type = EOL_TOKEN;
                    return OK;
                }
                else if(c == EOF)
                {
                    token->type = EOF_TOKEN;
                    return OK;  
                }     
                else
                {
                    return LEX_ERR;
                }
                break;
            case COLON_STATE:
                if(c == '=')
                {
                   token->type = SHORT_VAR_DECLARATION_TOKEN;
                   return OK;
                }
                else
                {
                    return LEX_ERR;
                }
                break;
            case EQUALS_ONCE_STATE:
                if(c == '=')
                {
                    token->type = EQUALS_TOKEN;
                }
                else
                {
                    ungetc(c,source_file);
                    token->type = ASSIGNMENT_TOKEN;
                }
                return OK;
                break;
            case EXCLAMATION_MARK_STATE:
                if(c == '=')
                {
                    token->type = NOT_EQUALS_TOKEN;
                    return OK;
                }
                else
                {
                    return LEX_ERR;
                }
                break;
            case LESS_STATE:
                if(c == '=')
                {
                    token->type = LESS_EQUAL_TOKEN;
                }
                else
                {
                    ungetc(c, source_file);
                    token->type = LESS_TOKEN;
                }
                return OK;
                break;
            case GREATER_STATE:
                if(c == '=')
                {
                    token->type = GREATER_EQUAL_TOKEN;
                }
                else
                {
                    ungetc(c, source_file);
                    token->type = GREATER_TOKEN;
                }
                return OK;
                break;
            case SLASH_STATE:
                if(c == '/')
                {
                    scanner_state = ONE_LINE_COMMENT_STATE;
                }
                else if(c == '*')
                {
                    scanner_state == BLOCK_COMMENT_STATE;
                }
                else
                {
                    ungetc(c,source_file);
                    token->type = DIVISON_TOKEN;
                    return OK;
                }
                break;
            case ID_KEYWORD_STATE:
                if(isalnum(c))
                {
                    strAddChar(token->str,c);
                }
                else
                {
                    ungetc(c,source_file);
                    keyword keyword_from_str;
                    if(str_is_keyword(token->str,&keyword_from_str))
                    {
                        strClear(token->str);
                        token->type = KEYWORD_TOKEN;
                        token->keyword = keyword_from_str;
                    }
                    else
                    {
                        token->type = ID_TOKEN;
                    }
                    return OK;
                    
                }
                
                break;
        }
    }
}

int main()
{
    
    return 0;
}


