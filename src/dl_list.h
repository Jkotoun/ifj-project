/****
 * Implementace překladače imperativního jazyka IFJ20.
 * Tomáš Beneš
 * Doubly linked list
*/

#include<stdio.h>
#include<stdlib.h>

#define FALSE 0
#define TRUE 1
 
typedef struct tElem {              
        int data;                    
        struct tElem *lptr;        
        struct tElem *rptr;       
} *tElemPtr;

typedef struct {                         
    tElemPtr First;                   
    tElemPtr Act;                     
    tElemPtr Last;                    
} tDLList;


void DLInitList (tDLList *);
void DLDisposeList (tDLList *);
void DLInsertFirst (tDLList *, int);
void DLInsertLast(tDLList *, int);
void DLFirst (tDLList *);
void DLLast (tDLList *);
void DLCopyFirst (tDLList *, int *);
void DLCopyLast (tDLList *, int *);
void DLDeleteFirst (tDLList *);
void DLDeleteLast (tDLList *);
void DLPostDelete (tDLList *);
void DLPreDelete (tDLList *);
void DLPostInsert (tDLList *, int);
void DLPreInsert (tDLList *, int);
void DLCopy (tDLList *, int *);
void DLActualize (tDLList *, int);
void DLSucc (tDLList *);
void DLPred (tDLList *);
int DLActive (tDLList *);