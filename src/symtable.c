/****
 * Implementace překladače imperativního jazyka IFJ20.
 * Josef Kotoun - xkotou06
 * Implementation of symtable
 * Some functions for tree data type are used from my implementation of IAL 2. project
 *
*/
#include "symtable.h"
#include "error_codes.h"
#include <string.h>
int symtable_init(symbol_node **rootptr)
{
    *rootptr = NULL;
    return OK;
}

bool symtable_search(symbol_node **rootptr, string *name, symbol_node **found_node)
{
    symbol_node *current = *rootptr;
    while (current != NULL)
    {
        int cmp_result = strCmpString(name, current->name);
        if (cmp_result > 0) //right subtree
        {
            current = current->r_ptr;
        }
        else if (cmp_result < 0) //left subtree
        {
            current = current->l_ptr;
        }
        else //found
        {
            *found_node = current;
            return true;
        }
    }
    return false;
}
symbol_node **getNodeToInsert(symbol_node *rootptr, string *name)
{

    symbol_node *current = rootptr;
    do
    {
        int cmp_result = strCmpString(name, current->name);
        if (cmp_result > 0)
        {
            //value is greater than node key and rptr is null => store new node into rptr
            if (current->r_ptr == NULL)
            {
                return &(current->r_ptr);
            }
            current = current->r_ptr;
        }
        else if (cmp_result < 0)
        {
            //value is smaller than node key and lptr is null => store new node into lptr
            if (current->l_ptr == NULL)
            {
                return &(current->l_ptr);
            }
            current = current->l_ptr;
        }
        else
        {
            //node is already in table - do nothing
            return NULL;
        }
    } while (current != NULL);
    return NULL;
}
int symtable_insert_node_func(symbol_node **rootptr, string *name, unsigned return_types_count, varType return_types[], unsigned params_count, varType params_types[], bool defined)
{
    symbol_node **ptrToNodePtr;
    if (*rootptr == NULL)
    {
        ptrToNodePtr = rootptr;
    }
    else
    {
        ptrToNodePtr = getNodeToInsert(*rootptr, name);
    }
    //symbol with name is already in tree - do nothing
    if (ptrToNodePtr == NULL)
    {
        return OK;
    }
    //allocation of  node
    (*ptrToNodePtr) = (symbol_node *)malloc(sizeof(symbol_node));
    if (*ptrToNodePtr == NULL)
    {
        return INTERNAL_COMPILER_ERR;
    }
    (*ptrToNodePtr)->l_ptr = (*ptrToNodePtr)->r_ptr = NULL;
    string *new_name = (string *)malloc(sizeof(string));
    if (strInit(new_name) == STR_ERROR)
    {
        return INTERNAL_COMPILER_ERR;
    }
    if (strCopyString(new_name, name) == STR_ERROR)
    {
        return INTERNAL_COMPILER_ERR;
    }
    (*ptrToNodePtr)->name = new_name;
    (*ptrToNodePtr)->type = func;

    //allocation of structure with function data
    (*ptrToNodePtr)->data = malloc(sizeof(symbol_function));
    if ((*ptrToNodePtr)->data == NULL)
    {
        free(*ptrToNodePtr);
        return INTERNAL_COMPILER_ERR;
    }
    symbol_function *data_ptr = (symbol_function *)(*ptrToNodePtr)->data;
    data_ptr->defined = defined;
    data_ptr->par_count = params_count;

    //allocation of array of param types
    data_ptr->parameters = (varType *)malloc(sizeof(varType) * params_count);
    if (data_ptr->parameters == NULL)
    {
        free(*ptrToNodePtr);
        free((*ptrToNodePtr)->data);
        return INTERNAL_COMPILER_ERR;
    }
    for (int i = 0; i < params_count; i++)
    {
        data_ptr->parameters[i] = params_types[i];
    }
    data_ptr->return_types_count = return_types_count;

    //allocation of array of return types
    data_ptr->return_types = (varType *)malloc(sizeof(varType) * return_types_count);
    if (data_ptr->return_types == NULL)
    {
        free(*ptrToNodePtr);
        free((*ptrToNodePtr)->data);
        free(data_ptr->parameters);
        return INTERNAL_COMPILER_ERR;
    }
    for (int i = 0; i < return_types_count; i++)
    {
        data_ptr->return_types[i] = return_types[i];
    }
    return OK;
}

int symtable_insert_node_var(symbol_node **rootptr, string *name, varType var_type)
{
    symbol_node **ptrToNodePtr;
    if (*rootptr == NULL)
    {
        ptrToNodePtr = rootptr;
    }
    else
    {
        ptrToNodePtr = getNodeToInsert((*rootptr), name);
    }
    //symbol with name is already in tree - do nothing
    if (ptrToNodePtr == NULL)
    {
        return OK;
    }
    (*ptrToNodePtr) = (symbol_node *)malloc(sizeof(symbol_node));
    if (*ptrToNodePtr == NULL)
    {
        return INTERNAL_COMPILER_ERR;
    }
    (*ptrToNodePtr)->l_ptr = (*ptrToNodePtr)->r_ptr = NULL;

    string *new_name = (string *)malloc(sizeof(string));
    if (strInit(new_name) == STR_ERROR)
    {
        return INTERNAL_COMPILER_ERR;
    }
    if (strCopyString(new_name, name) == STR_ERROR)
    {
        return INTERNAL_COMPILER_ERR;
    }
    (*ptrToNodePtr)->name = new_name;
    (*ptrToNodePtr)->type = var;
    (*ptrToNodePtr)->data = (symbol_variable *)malloc(sizeof(symbol_variable));
    if ((*ptrToNodePtr)->data == NULL)
    {
        free(*ptrToNodePtr);
        return INTERNAL_COMPILER_ERR;
    }
    ((symbol_variable *)(*ptrToNodePtr)->data)->var_type = var_type;
    return OK;
}

bool symtable_contains_undef_func(symbol_node **rootptr)
{

    if (*rootptr != NULL)
    {
        symbol_function *func_data = (symbol_function *)((*rootptr)->data);
        if (!func_data->defined)
        {
            return true;
        }
        if (symtable_contains_undef_func(&((*rootptr)->l_ptr)) || symtable_contains_undef_func(&((*rootptr)->r_ptr)))
        {
            return true;
        }
    }
    return false;
}

int symtable_free_node_memory(symbol_node *nodeptr)
{
    if (nodeptr != NULL)
    {
        if (nodeptr->type == func)
        {
            symbol_function *data = (symbol_function *)nodeptr->data;
            //free params and return types arrays for function
            free(data->parameters);
            free(data->return_types);
        }
        if (nodeptr->data != NULL)
        {
            free(nodeptr->data);
        }
        if (nodeptr->name != NULL)
        {
            //free string->char*
            strFree(nodeptr->name);
            //free allocated string struct
            free(nodeptr->name);
        }
        free(nodeptr);
    }
    return OK;
}

int symtable_dispose_tree(symbol_node **rootptr)
{
    if (*rootptr != NULL)
    {
        if ((*rootptr)->l_ptr != NULL)
        {
            if (symtable_dispose_tree(&((*rootptr)->l_ptr)) != OK)
            {
                return INTERNAL_COMPILER_ERR;
            }
        }
        if ((*rootptr)->r_ptr != NULL)
        {
            if (symtable_dispose_tree(&((*rootptr)->r_ptr)) != OK)
            {
                return INTERNAL_COMPILER_ERR;
            }
        }
        symtable_free_node_memory(*rootptr);
        *rootptr = NULL;
        return OK;
    }
    else
    {
        return INTERNAL_COMPILER_ERR;
    }
}