/****
 * Implementace překladače imperativního jazyka IFJ20.
 * Josef Kotoun - xkotou06
 * Header file of symtable
 *
*/
#ifndef symtable_h
#define symtable_h
#include "dl_list.h"
#include "str.h"
#include <stdbool.h>

typedef enum varType
{
	INT,
	STRING,
	FLOAT,
	BOOL,
	UNDEFINED,
	UNDERSCORE //_ var can be used in place of all types
}varType;

typedef enum node_type
{
	func,
	var
} node_type;

//tree node - function (global table) or variable (local table)
typedef struct symbol_node
{
	string* name;
	node_type type;			   //func or var
	struct symbol_node* l_ptr; //tree links
	struct symbol_node* r_ptr;
	void* data; //pointer to symbol_function or symbol_variable
} symbol_node;

//item of symtables list for function (1 table for each scope)
typedef struct table
{
	symbol_node* root_ptr;
	int scope_index; //index of symtable scope indexed from 0
	struct table* prev_table;
	struct table* next_table;
} table;

//function data in global symtable
typedef struct
{
	unsigned return_types_count; //func return type
	varType* return_types;		 //pointer to array of return data types
	unsigned par_count;			 //func params count
	varType* parameters;		 //Pointer to array of params data types
	bool defined;				 //found definition of function
} symbol_function;

//variable data in local symtable
typedef struct
{
	varType var_type; //data type of variable
} symbol_variable;

//inits rootptr to NULL
int symtable_init(symbol_node** rootptr);
//returns if node with name is in tree and if yes, stores pointer to  found_node
bool symtable_search(symbol_node** rootptr, string* name, symbol_node** found_node);
//insert node into global function table
int symtable_insert_node_func(symbol_node** rootptr, string* name, unsigned return_type_count, varType return_types[], unsigned params_count, varType params_types[], bool defined);
//insert node into local variable table
int symtable_insert_node_var(symbol_node** rootpr, string* name, varType var_type);
//checks if any function from global functions symtable is undefined
bool symtable_contains_undef_func(symbol_node** rootptr);
//free all memory for all types of nodes
int symtable_free_node_memory(symbol_node* nodeptr);
//dispose tree
int symtable_dispose_tree(symbol_node** rootptr);
#endif