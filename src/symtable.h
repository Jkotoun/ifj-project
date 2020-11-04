#ifndef symtable_h
#define symtable_h
#include "dl_list.h"

typedef enum varType
{
	INT,
	STRING,
	FLOAT
}varType;
typedef enum node_type
{
	func,
	var
}node_type;
//item of symtables list for function (1 table for each scope)
typedef struct table
{
	node *root_ptr;
	table *prev_table;
	table *next_table;
	int scope_index;
} table;

//tree node - function (global table) or variable (local table)
typedef struct node
{
	char *name; 
	node_type type; //func or var
	struct node *l_ptr; //tree links
	struct node *r_ptr;
	void *data; //pointer to symbol_function or symbol_variable
} node;

//function data in global symtable
typedef struct
{
	unsigned type_count; //func return type
	varType *return_types;//pointer to array of return data types
	unsigned par_count;	 //func params count
	varType *parameters; //Pointer to array of params data types
	bool defined;		 //found definition of function
} symbol_function;

//variable data in local symtable
typedef struct
{
	varType var_type; //data type of variable
} symbol_variable;

node function_table;
tDLList list; //list of sym_tables for this function


void init(node *);
node search(node, char, int *);
symbol_function *create_func_node();
symbol_function *create_var_node();

void insert_node(node *, char, int);
void delete_node(node *, char);
void dispose(node *);
#endif