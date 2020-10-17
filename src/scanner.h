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
    ID,
    KEYWORD,
    EOL_TYPE,
    EOF_TYPE,
    SHORT_VAR_DECLARATION,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    CURLY_BRACKET_LEFT,
    CURLY_BRACKET_RIGHT,
    PLUS,
    MINUS,
    MULTIPLICATION,
    DIVISON,
    COMMA,
    ASSIGNMENT,
    SEMICOLON,
    LESS,
    LESS_EQUAL,
    GREATER,
    GREATER_EQUAL,
    EQUALS,
    NOT_EQUALS,
    DECIMAL_LITERAL,
    INTEGER_LITERAL
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
    COLON_STATE,
    EXCLAMATION_MARK_STATE,
    GREATER_STATE,
    LESS_STATE,
    INT_STATE,
    REAL_POINT_STATE,
    REAL_NO_EXP_STATE,
    EXP_STATE,
    REAL_WITH_EXP_STATE,
    ID_KEYWORD_STATE,
    ID_STATE,
    STRING_STATE,
    ESCAPE_STRING_STATE,
    HEXA_ESCAPE_START_STATE,
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
    union
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

//sets source file of program to compile
void setSourceFile(FILE* file);

#endif