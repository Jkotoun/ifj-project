/****
 * Implementace překladače imperativního jazyka IFJ20.
 * Josef Kotoun - xkotou06
 * Header file for double linked list for local symtables of function (1 for each scope)
 * Interface of some functions are used from my implementation of IAL first project
 * 
*/
#ifndef dl_list_h
#define dl_list_h
#include<stdio.h>
#include<stdlib.h>
#include "symtable.h"


//pointer to first and last element of dl_list
typedef struct tDLList{  
    struct table* First;  
    struct table* Last;  
} tDLList;

typedef struct node node;

//inits first and last pointer to NULL                              
int DLInitList (tDLList* list);
//delete last table from list and free its memory
int DLDeleteLast (tDLList* list);
//delete all items and free memory
int DLDisposeList (tDLList* list);
//insert new table to end of list (new scope - scope index is calculated and tree ptr initialized to NULL)
int DLInsertLast(tDLList* list);

#endif