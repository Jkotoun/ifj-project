#pragma once

typedef struct node {
	char name;	
	node_type type;
	void* ptr;
	struct node *l_ptr;
	struct node *r_ptr;
} *node_ptr;

enum node_type {
	func;
	var;
};

void init(node_ptr*);
node_ptr search(node, char, int*);
void insert_node(node_ptr*, char, int);
void delete_node(node_ptr*, char);
void dispose(node_ptr*);

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