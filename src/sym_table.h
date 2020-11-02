#pragma once

#include "dl_list.h"

enum varType
{
	INT,
	STRING,
	FLOAT
};

typedef struct table
{
	node *root_ptr;
	table *prev_table;
	table *next_table;
	int scope_index;
} table;

/* tree node*/
typedef struct node
{
	char *name;
	node_type type;
	struct node *l_ptr; //tree links
	struct node *r_ptr;
	void *data; //pointer to symbol_function or symbol_variable
} node;

/* function symbol */
typedef struct
{
	unsigned type_count; //Navratovy typ funkce.
	varType *return_types;
	unsigned par_count;	 //Pocet parametru.
	varType *parameters; //Ukazatel na pole promennych.
	bool defined;		 //Informuje, zda byla jiz funkce definovana.
} symbol_function;

/* variable symbol */
typedef struct
{
	unsigned type;	//Datovy typ promenne.
	unsigned index; //Ciselny identifikator reprezentujici promennou na ramci.e.
	varType var_type;
} symbol_variable;

node function_table;
tDLList list; //list of sym_tables for this function

enum node_type
{
	func,
	var
};

void init(node *);
node search(node, char, int *);
symbol_function *create_func_node();
symbol_function *create_var_node();

void insert_node(node *, char, int);
void delete_node(node *, char);
void dispose(node *);