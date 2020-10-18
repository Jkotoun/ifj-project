/****
 * Implementace překladače imperativního jazyka IFJ20.
 * Josef Kotoun - xkotou06
 * Lexical analysis (scanner)
*/
#ifndef scanner_h
#define scanner_h
#include "str.h"
#include <stdbool.h>
#include <stdint.h>


//types of tokens
typedef enum token_type
{
    ID_TOKEN,
    KEYWORD_TOKEN,
    STRING_TOKEN,
    EOL_TOKEN,
    EOF_TOKEN,
    SHORT_VAR_DECLARATION_TOKEN,
    LEFT_BRACKET_TOKEN,
    RIGHT_BRACKET_TOKEN,
    CURLY_BRACKET_LEFT_TOKEN,
    CURLY_BRACKET_RIGHT_TOKEN,
    PLUS_TOKEN,
    MINUS_TOKEN,
    MULTIPLICATION_TOKEN,
    DIVISON_TOKEN,
    COMMA_TOKEN,
    ASSIGNMENT_TOKEN,
    SEMICOLON_TOKEN,
    LESS_TOKEN,
    LESS_EQUAL_TOKEN,
    GREATER_TOKEN,
    GREATER_EQUAL_TOKEN,
    EQUALS_TOKEN,
    NOT_EQUALS_TOKEN,
    DECIMAL_LITERAL_TOKEN,
    INTEGER_LITERAL_TOKEN,
}token_type;

//all keywords in IFJ20
typedef enum keyword
{
    ELSE_KEYWORD,
    FLOAT64_KEYWORD,
    FOR_KEYWORD,
    FUNC_KEYWORD,
    IF_KEYWORD,
    INT_KEYWORD,
    PACKAGE_KEYWORD,
    RETURN_KEYWORD,
    STRING_KEYWORD
}keyword;
//scanner finite automaton states
typedef enum state
{
    INITIAL_STATE,
    EQUALS_ONCE_STATE,
    COLON_STATE,
    EXCLAMATION_MARK_STATE,
    GREATER_STATE,
    LESS_STATE,
    NUM_STATE,
    DECIMAL_POINT_STATE,
    DECIMAL_NO_EXP_STATE,
    EXP_DEC_STATE,
    DECIMAL_WITH_EXP_STATE,
    ID_KEYWORD_STATE,
    STRING_STATE,
    ESCAPE_STRING_STATE,
    HEXA_ESCAPE_FIRST_STATE,
    SLASH_STATE,
    ONE_LINE_COMMENT_STATE,
    BLOCK_COMMENT_STATE,
    BLOCK_COMMENT_STAR_STATE
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
    struct
    {
        int64_t integer;
        double decimal;
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


#endif