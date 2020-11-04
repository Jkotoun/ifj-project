/****
 * Implementace překladače imperativního jazyka IFJ20.
 * Tomáš Beneš
 * Doubly linked list
*/

#include "dl_list.h"

void DLError() {
    printf ("*ERROR* The program has performed an illegal operation.\n");
    return;
}

void DLInitList (tDLList *L) {
    L->Act=NULL;
	L->First=NULL;
    L->Last=NULL;
}

void DLDisposeList (tDLList *L) {
    L->Act=NULL;
    L->Last=NULL;
	
    while(L->First!=NULL){
        tElemPtr deleted=L->First;  // Uložení si druhého prvku seznamu.
        L->First=L->First->rptr;    // Nastavení druhého prvku seznamu, jako první prvek seznamu.
        free(deleted);              // Uvolnění paměti prvního prvku seznamu.
    }
}

void DLInsertFirst (tDLList *L, int val) {
    tElemPtr newElement= (tElemPtr)malloc(sizeof(struct tElem));
    if(newElement==NULL){
        DLError();
    } else {
        newElement->data=val;
        newElement->lptr=NULL;
        newElement->rptr=NULL;

        if(L->First!=NULL){
            newElement->rptr=L->First;  // Následovník nového elementu bude ten, co je aktuálně první.
            L->First->lptr=newElement;  // Předchůdce aktuálně prvního elementu bude nový element.
            L->First=newElement;        // Nový element je nastaven jako první element v seznamu.

        }else{                          // Pokud bude seznam prázdný, tak se uloží nový element jak do First tak do Last.
            L->First=newElement;
            L->Last=newElement;
        }
    }
}

void DLInsertLast(tDLList *L, int val) {	
	tElemPtr newElement= (tElemPtr)malloc(sizeof(struct tElem));
    if(newElement==NULL){
        DLError();
    } else {
        newElement->data=val;
        newElement->lptr=NULL;
        newElement->rptr=NULL;

        if(L->Last!=NULL){
            newElement->lptr=L->Last;  // Předchůdce nového elementu bude ten, co je aktuálně poslední.
            L->Last->rptr=newElement;  // Následovník aktuálně posledního elementu bude nový element.
            L->Last=newElement;        // Nový element je nastaven jako poslední element v seznamu.

        }else{                         // Pokud bude seznam prázdný, tak se uloží nový element jak do First tak do Last.
            L->First=newElement;
            L->Last=newElement;
        }
    }
}

void DLFirst (tDLList *L) {
    L->Act=L->First;
}

void DLLast (tDLList *L) {
	L->Act=L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
    if(L->First==NULL){
        DLError();
    } else {
        *val=L->First->data;
    }
}

void DLCopyLast (tDLList *L, int *val) {
    if(L->Last==NULL){
        DLError();
    } else {
        *val=L->Last->data;
    }
}

void DLDeleteFirst (tDLList *L) {
	if(L->First!=NULL){
        if(L->Act==L->First){
            L->Act=NULL;
        }
        if(L->First==L->Last){
            free(L->First);
            L->First=NULL;
            L->Last=NULL;
        }else{
            tElemPtr tmp=L->First;
            L->First=L->First->rptr;    // Nastavení druhého prvku, jako první prvek seznamu.
            L->First->lptr=NULL;        // Nastavení předchůdce nového prvního elementu na NULL.
            free(tmp);                  // Uvolnění paměti.
        }
    }
}	

void DLDeleteLast (tDLList *L) {
	if(L->Last!=NULL){
        if(L->Act==L->Last){
            L->Act=NULL;
        }
        if(L->First==L->Last){
            free(L->Last);
            L->First=NULL;
            L->Last=NULL;
        }else{
            tElemPtr tmp=L->Last;
            L->Last=L->Last->lptr;      // Nastavení druhého prvku od zadu, jako poslední prvek seznamu.
            L->Last->rptr=NULL;         // Nastavení náledovníka nového posledního elementu na NULL.
            free(tmp);                  // Uvolnění paměti.
        }
    }
}

void DLPostDelete (tDLList *L) {
	if (L->Act!=NULL&&L->Act!=L->Last)
    {
        tElemPtr tmp=L->Act->rptr;
        L->Act->rptr=L->Act->rptr->rptr;    // Nastavení následovníka aktivního prvku na prvek následuhící odebíraný prvek.
        if(L->Act->rptr!=NULL){             // Pokud aktuální následovník není NULL, tak
            L->Act->rptr->lptr=L->Act;      // se nastaví aktivní prvek jako předchůdce.
        }else{
            L->Last=L->Act;
        }
        free(tmp);                          // Uvolnění paměti.
    }
}

void DLPreDelete (tDLList *L) {
	if (L->Act!=NULL&&L->Act!=L->First)
    {
        tElemPtr tmp=L->Act->lptr;
        L->Act->lptr=L->Act->lptr->lptr;    // Nastavení předchůdce aktivního prvku na prvek předcházející odebíraný prvek.
        if(L->Act->lptr!=NULL){             // Pokud aktuální předchůdce není NULL, tak
            L->Act->lptr->rptr=L->Act;      // se nastaví aktivní prvek jako následovník.
        }else{
            L->First=L->Act;
        }
        free(tmp);                          // Uvolnění paměti.
    }
}

void DLPostInsert (tDLList *L, int val) {
	if(L->Act!=NULL){
        tElemPtr newElement=(tElemPtr)malloc(sizeof(struct tElem));
        if (newElement==NULL){
            DLError();
        }else{
            newElement->data=val;
            newElement->lptr=L->Act;
            newElement->rptr=L->Act->rptr;
            L->Act->rptr=newElement;
            if(newElement->rptr!=NULL){
                newElement->rptr->lptr=newElement;
            }else{
                L->Last=newElement;
            }
            
        }
    }
}

void DLPreInsert (tDLList *L, int val) {
	if(L->Act!=NULL){
        tElemPtr newElement=(tElemPtr)malloc(sizeof(struct tElem));
        if (newElement==NULL){
            DLError();
        }else{
            newElement->data=val;
            newElement->rptr=L->Act;
            newElement->lptr=L->Act->lptr;
            L->Act->lptr=newElement;
            if(newElement->lptr!=NULL){
                newElement->lptr->rptr=newElement;
            }else{
                L->First=newElement;
            }
        }
    }
}

void DLCopy (tDLList *L, int *val) {
	if(L->Act==NULL){
        DLError();
    }else{
        *val=L->Act->data;
    }
}

void DLActualize (tDLList *L, int val) {
	if(L->Act!=NULL){
        L->Act->data=val;
    }
}

void DLSucc (tDLList *L) {
	if (L->Act!=NULL)
    {
        L->Act=L->Act->rptr;
    }
}


void DLPred (tDLList *L) {
	if (L->Act!=NULL)
    {
        L->Act=L->Act->lptr;
    }
}

int DLActive (tDLList *L) {
	return (L->Act!=NULL);
}