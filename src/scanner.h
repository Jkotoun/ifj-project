/****
 * Implementace překladače imperativního jazyka IFJ20.
 * Josef Kotoun - xkotou06
 * Lexical analysis (scanner)
*/
#ifndef scanner_h
#define scanner_h
#include "str.h"
#include <stdbool.h>



//types of tokens
typedef enum token_type
{
    ID,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    PLUS,
    MINUS,
    EOL_type,
    EOF_type
}token_type;

//all keywords in IFJ20
typedef enum keyword
{
    else_keyword,
    int_keyword
}keyword;
//scanner finite automaton states
typedef enum state
{
    initial_state,
    keyword_or_id,

}state;

//pairs of keyword and its string representation
typedef struct keyword_str_pair
{
    keyword keyword;
    char* keyword_str;
}keyword_str_pair;


typedef struct token
{
    token_type type;
    union
    {
        int integer;
        string *str;
        keyword keyword;
    };
}token;

//*token - output token pointer - contains information about scanned lexem
//returns TOKEN_OK value if token is valid, LEX_ERR if invalid 
int get_token(token *token);

//returns whether the string is keyword
//if str is keyword, passes it to *keyword parameter
bool str_is_keyword(string *str, keyword *keyword);

//sets source file of program to compile
void setSourceFile(FILE* file);

#endif