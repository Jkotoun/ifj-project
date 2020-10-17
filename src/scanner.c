/****
 * Implementace překladače imperativního jazyka IFJ20.
 * Josef Kotoun - xkotou06
 * Lexical analysis (scanner)
*/
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "str.h"
#include "scanner.h"

static keyword_str_pair all_keywords_pairs[] = 
{
    {int_keyword, "int"},
    {else_keyword, "else"}
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
    string lexem;
    state scanner_state = initial_state;
    while(1)
    {   
        c = getc(source_file);
        switch(scanner_state)
        {
            case initial_state:

            break;
        }
    }
    return 0;
}
/*int main()
{
    string str;
    strInit(&str);
    strAddChar(&str,'i');
    strAddChar(&str,'f');
    strAddChar(&str,'t');
    Keyword keyword;
    bool is_keyword = str_is_keyword(&str,&keyword);

    is_keyword ? printf("%s", "jojojo") : printf("%s", "nenene"); 
    strFree(&str);
    return 0;
}*/

