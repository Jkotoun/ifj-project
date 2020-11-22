
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
int strInit(string* s);
void strFree(string* s);
void strClear(string* s);
int strAddChar(string* s1, char c);
int strCopyString(string* s1, string* s2);
int strCmpString(string* s1, string* s2);
int strCmpConstStr(string* s1, char* s2);
char* strGetStr(string* s);
int strGetLength(string* s);

//concatentates strings s1 and s2 to dest concat_str
int strConcat(string* s1, string* s2, string* concat_str);

//str = source string, index = begining of substring, count = lenght of substring, output_str = output substring
//return 1 if index is out of range
//output substring is number of chars from index or chars from index to end of string, if index + count is out of range
int substr(string* str, int index, int count, string* output_str);

//stores ASCII value of char at given index to *output_ascii
//returns 0 on succes, 1 if index is out of range 
int str_ord(string* str, int index, int* output_ascii);

//stores given ascii value of char as 1 char string (to output string)
//return 0 on success, 1 if ASCII value is out of <0,255>
int ascii_to_string(int ascii, string* output);
//add char* to string (char* has to end with \0)
int strAddConstStr(string* str, char* source);
#endif
