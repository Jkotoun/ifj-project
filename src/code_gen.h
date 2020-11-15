/****
 * Implementace překladače imperativního jazyka IFJ20.
 * Tomáš Beneš - xbenes55
 * Generator of output code
*/
#include "str.h"

#ifndef code_gen_h
#define code_gen_h

// Dynamic array for identifying how much if/while/for is in the scopes ----------------------
#define ALLOCATE_CHUNK 8

typedef struct dArray
{
    int* count_in_scope;
    int size_of_array;
}dArray;
     
int dArray_init(dArray* array);                     // Alocating first count_of_scopes, and set default values
                                                    // Return 0 if the task was succesfull
                                                    // otherwise return 1

int dArray_add_to_scope(dArray* array, int index);  // Increment count_in_scope[index] by 1
                                                    // Return 0 if the task was succesfull
                                                    // otherwise return 1

int dArray_despose(dArray* array);                  // Free memory of array
// -------------------------------------------------------------------------------------------


// Generator of 3AC --------------------------------------------------------------------------
#define MAX_DIGITS_OF_SCOPE 48

// Initiation of generator
int generator_init();
int generate_build_in_function(string *output);              
void generator_print_output();

// Main
int genetate_main_start();
int genetate_main_end();        //<-- návěští pro předčasné ukončení mainu + return mainu


// Generating for variables
int generate_new_var();
int generate_assign_var_to_var();
int generate_assign_const_to_var();

// Build-in functions
int generate_inputs();
int generate_inputi();
int generate_inputf();
int generate_print();
int generate_int2float();
int generate_float2int();
int generate_len();
int generate_substr();
int generate_ord();
int generate_chr();

// Generating sequential and selection instruction
// -- Generating body of if else
int generate_if_start(int scope, char *name_of_function);
int generate_if_else(int scope, char *name_of_function);
int generate_if_end(int scope, char *name_of_function);
// -- Generating boy of while loop
int generate_while_start();
int generate_while_end();
// -- Generating body of for loop
int generate_for_start();
int generate_for_end();
// -- Generating body of function
int generate_function_start();
int generate_function_end();

// Calling function
int generate_call_function(char *function_name);
int generate_return_function();

// Cleaning inner string
void generator_clear();

// -------------------------------------------------------------------------------------------

enum instruction_type
{
    // Func calls, frames
    MOVE,
    CREATEFRAME,
    PUSHFRAME,
    POPFRAME,
    DEFVAR,
    CALL,
    RETURN,

    // Stack
    PUSHS,
    POPS,
    CLEARS,

    // Aritmetic instructions
    ADD,
    SUB,
    MUL,
    DIV,
    IDIV,
    ADDS,
    SUBS,
    MULS,
    DIVS,
    IDIVS,

    // Relational instructions
    LT,
    GT,
    EQ,
    LTS,
    GTS,
    EQS,

    // Bool instructions
    AND,
    OR,
    NOT,
    ANDS,
    ORS,
    NOTS,

    // Conversion instructions
    INT2FLOAT,
    FLOAT2INT,
    INT2CHAR,
    STRI2INT,
    INT2FLOATS,
    FLOAT2INTS,
    INT2CHARS,
    STRI2INTS,

    // I/0 instructions
    READ,
    WRITE,

    // String instructions
    CONCAT,
    STRLEN,
    GETCHAR,
    SETCHAR,

    // Types
    TYPE,

    // Program flow instructions
    LABEL,
    JUMP,
    JUMPIFEQ,
    JUMPIFNEQ,
    JUMPIFEQS,
    JUMPIFNEQS,
    EXIT,

    // Debugging instructions
    BREAK,
    DPRINT
};
#endif