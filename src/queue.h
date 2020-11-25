/****
 * Implementace překladače imperativního jazyka IFJ20.
 * Jiří Vlasák - xvlasa15
 * Fronta tokenů a typů - upraveno z 1. DU do IAL.
*/

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "scanner.h"
#include "symtable.h"
#include <stdio.h>

#define MAX_QUEUE 50
// int QUEUE_SIZE = MAX_QUEUE;
int err_flag; /* Indikuje, zda operace volala chybu. */

/*
 * Hodnota MAX_QUEUE udává skutečnou velikost statického pole pro uložení
 * hodnot fronty. Při implementaci operací nad ADT fronta však předpokládejte,
 * že velikost tohoto pole je pouze QUEUE_SIZE. Umožní nám to jednoduše
 * měnit velikost fronty v průběhu testování. Při implementaci ADT fronta byste
 * tedy hodnotu MAX_QUEUE neměli vůbec použít. Pamatujte, že do fronty se vejde
 * maximálně (QUEUE_SIZE - 1) prvků. Jedna pozice ve frontě bude vždy nevyužitá,
 * aby bylo možné odlišit prázdnou frontu od plné.
 */

/* Chybové kódy pro funkci queueError */
#define MAX_QERR 5    /* počet možných chyb */
#define QERR_UP 1     /* chyba při stackTop */
#define QERR_FRONT 2  /* chyba při stackPop */
#define QERR_REMOVE 3 /* chyba při stackPush */
#define QERR_GET 4    /* chyba při stackPush */
#define QERR_INIT 5   /* chyba při malloc */

/* ADT fronta implementovaný ve statickém poli */
typedef struct
{
    string name;
    int scope;
} varT;

typedef struct
{
    varType arr[MAX_QUEUE]; /* pole pro uložení hodnot */
    int f_index;            /* index prvního prvku */
    int b_index;            /* index první volné pozice */
} typeQueue;

typedef struct
{
    token arr[MAX_QUEUE]; /* pole pro uložení hodnot */
    int f_index;          /* index prvního prvku */
    int b_index;          /* index první volné pozice */
} tokenQueue;

typedef struct
{
    varT arr[MAX_QUEUE]; /* pole pro uložení hodnot */
    int f_index;         /* index prvního prvku */
    int b_index;         /* index první volné pozice */
} varQueue;

/* Hlavičky funkcí pro práci s frontou. */
void queueError(int error_code);
void typeQueueInit(typeQueue* q);
int typeQueueEmpty(const typeQueue* q);
int typeQueueFull(const typeQueue* q);
void typeQueueFront(const typeQueue* q, varType* c);
void typeQueueRemove(typeQueue* q);
void typeQueueGet(typeQueue* q, varType* c);
void typeQueueUp(typeQueue* q, varType c);
varType* typeQueueToArray(typeQueue* q);
int typeQueueLength(typeQueue* q);

void tokenQueueInit(tokenQueue* q);
int tokenQueueEmpty(const tokenQueue* q);
int tokenQueueFull(const tokenQueue* q);
void tokenQueueFront(const tokenQueue* q, token* token);
void tokenQueueRemove(tokenQueue* q);
void tokenQueueGet(tokenQueue* q, token* token);
void tokenQueueUp(tokenQueue* q, token token);
token* tokenQueueToArray(tokenQueue* q);
int tokenQueueLength(tokenQueue* q);

void varQueueInit(varQueue* q);
int varQueueEmpty(const varQueue* q);
int varQueueFull(const varQueue* q);
void varQueueFront(const varQueue* q, varT* var);
void varQueueRemove(varQueue* q);
void varQueueGet(varQueue* q, varT* var);
void varQueueUp(varQueue* q, string* name, int scope);
varT* varQueueToArray(varQueue* q);
int varQueueLength(varQueue* q);

#endif