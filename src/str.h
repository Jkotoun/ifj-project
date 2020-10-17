/****
 * Implementace překladače imperativního jazyka IFJ20.
 * Josef Kotoun - xkotou06
 * Used from official example "simple interpret"
 * */
#ifndef str_h
#define str_h


typedef struct
{
  char* str;		
  int length;	
  int allocSize;	// size of currently allocated memory (in bytes)
} string;


int strInit(string *s);
void strFree(string *s);

void strClear(string *s);
int strAddChar(string *s1, char c);
int strCopyString(string *s1, string *s2);
int strCmpString(string *s1, string *s2);
int strCmpConstStr(string *s1, char *s2);

char *strGetStr(string *s);
int strGetLength(string *s);
#endif


//rozsireni - konkatenace - pokud je v projektu na to funkce, ?vyhledavani v podretezci? 
