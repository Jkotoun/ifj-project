/****
 * Implementace překladače imperativního jazyka IFJ20.
 * Josef Kotoun - xkotou06
 * Double linked list for local symtables of function (1 for each scope)
 * Interface and implementation of some functions are used from my implementation of IAL first project
*/
#include "dl_list.h"
#include "error_codes.h"


int DLInitList (tDLList *list) {
    if(list == NULL)
    {
        return INTERNAL_COMPILER_ERR;
    }
    list->First = NULL;
    list->Last = NULL;
}

int DLDisposeList (tDLList *list) {
    if(list == NULL)
    {
        return INTERNAL_COMPILER_ERR;
    }
	table* current = list->First;
    //iterate over all items
    while(current!=NULL)
    {
        table* next_element = current->next_table;
        //delete tree linked with table
        dispose(current->root_ptr);
        free(current);
        current = next_element;
    }
    list->First = NULL;
    list->Last = NULL;
}


int DLInsertLast(tDLList *list, node* rootptr) {
	table* new = (table*)malloc(sizeof(table));
    if(new == NULL)
    {
        return INTERNAL_COMPILER_ERR;
    }
    else
    {
        new->root_ptr = rootptr;
        new->next_table= NULL;
        new->prev_table = list->Last;
        if(list->Last == NULL)
        {
            list->First = new;
        }
        else
        {
            list->Last->next_table = new;
            
        }
        list->Last = new;
    }
}


int DLDeleteLast (tDLList *list) {
    if(list == NULL)
    {
        return INTERNAL_COMPILER_ERR;
    }
    //list has any elements
	if(list->First != NULL)
    {
        table* tmp = list->Last;
        //list has 1 element
        if(list->Last == list->First)
        {
            list->First = NULL;
            list->Last = NULL;
        } 
        else
        {
            //set prev element rptr to null - 
            tmp->prev_table->next_table = NULL;
            //set prev element as last and free memory
            list->Last = tmp->prev_table;
        }
        //delete tree linked to table
        dispose(tmp->root_ptr);
        //free table
        free(tmp);
    }
}