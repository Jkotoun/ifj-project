#pragma once

#include "dl_list.h"

enum varType
{
	INT,
	STRING,
	FLOAT
};

typedef struct node
{
	char *name;
	node_type type;
	struct node *l_ptr; //tree links
	struct node *r_ptr;
	void *data; //pointer to symbol_function or symbol_variable
} node;

typedef struct
{
	unsigned type_count; //Navratovy typ funkce.
	varType *return_types;
	unsigned par_count;	 //Pocet parametru.
	varType *parameters; //Ukazatel na pole promennych.
	bool defined;		 //Informuje, zda byla jiz funkce definovana.
	tDLList list;		 //list of sym_tables for this function
} symbol_function;

/* Struktura symbolu promenne. */
typedef struct
{
	unsigned type;	//Datovy typ promenne.
	unsigned index; //Ciselny identifikator reprezentujici promennou na ramci.e.
	varType var_type;
} symbol_variable;

node function_table;

enum node_type
{
	func,
	var
};

void init(node *);
node search(node, char, int *);
void insert_node(node *, char, int);
void delete_node(node *, char);
void dispose(node *);

main()()
{
}

/*		
	a := 1;
	b := 3;
	if(true){
		a := 2;
		a = b + a;
		if(true){
			a := 2;
			a = b + a;
		}
	}
	
	PUSHFRAME 
	CREATEFRAME
	DEFVAR a
	MOVE LF@a 1
	DEFVAR b
	MOVE LF@b ;

	PUSHFRAME
	CREATEFRAME
	DEFVAR a
	MOVE LF@a 2
	ADD LF@a LF@a GF@a
	POPFRAME
*/