/****
 * Implementace překladače imperativního jazyka IFJ20.
 * Josef Kotoun - xkotou06
 * Double linked list for local symtables of function (1 for each scope)
 * Interface and implementation of some functions are used from my implementation of IAL first project
*/
#include "dl_list.h"
#include "error_codes.h"

int DLInitList(tDLList *list)
{
    if (list == NULL)
    {
        return INTERNAL_COMPILER_ERR;
    }
    list->First = NULL;
    list->Last = NULL;
    return OK;
}

int DLDeleteLast(tDLList *list)
{
    //list has any elements
    if (list->First != NULL)
    {
        table *tmp = list->Last;
        //list has 1 element
        if (list->Last == list->First)
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
        if (tmp->root_ptr != NULL)
        {
            //delete tree linked to table
            symtable_dispose_tree(&(tmp->root_ptr));
        }
        //free table
        free(tmp);
    }
    return OK;
}
int DLDisposeList(tDLList *list)
{
    while (list->First != NULL)
    {
        DLDeleteLast(list);
    }
    return OK;
}

int DLInsertLast(tDLList *list)
{
    table *new = (table *)malloc(sizeof(table));
    if (new == NULL)
    {
        return INTERNAL_COMPILER_ERR;
    }
    else
    {
        // new->root_ptr = rootptr;
        new->next_table = NULL;
        new->prev_table = list->Last;
        symtable_init(&(new->root_ptr));
        if (list->Last == NULL)
        {
            new->scope_index = 0; //first var table for function
            list->First = new;
        }
        else
        {
            new->scope_index = list->Last->scope_index + 1; //table for scope has index greater by 1
            list->Last->next_table = new;
        }
        list->Last = new;
    }
    return OK;
}
