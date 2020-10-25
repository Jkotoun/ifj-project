/****
 * Implementace překladače imperativního jazyka IFJ20.
 * Josef Kotoun - xkotou06
 * Used from official example "simple interpret"
 * */
#ifndef str_h
#define str_h
#define STR_ERROR   1
#define STR_SUCCESS 0
#define STR_LEN_INC 8

typedef struct
{
  char* str;		
  int length;	
  int allocSize;	// size of currently allocated memory (in bytes)
} string;

//initial setup of string - alloc for 8 bytes, size 0
int strInit(string *s);
void strFree(string *s);
void strClear(string *s);
int strAddChar(string *s1, char c);
int strCopyString(string *s1, string *s2);
int strCmpString(string *s1, string *s2);
int strCmpConstStr(string *s1, char *s2);

char *strGetStr(string *s);
int strGetLength(string *s);
//concatentates strings s1 and s2 to dest concat_str
int strConcat(string *s1, string *s2, string *concat_str);
#endif


//rozsireni - konkatenace - pokud je v projektu na to funkce, ?vyhledavani v podretezci? 
