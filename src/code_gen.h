/****
 * Implementace překladače imperativního jazyka IFJ20.
 * Tomáš Beneš - xbenes55
 * Generator of output code
*/
#include "str.h"
#include "symtable.h"

#ifndef code_gen_h
#define code_gen_h

// Dynamic array for identifying how much if/while/for is in the scopes ----------------------
#define ALLOCATE_CHUNK 8
#define SIZE_OF_ASCII_ARRAY 35

typedef struct dArray
{
    int* count_in_scope;
    int size_of_array;
} dArray;

typedef enum instruction_type
{
    // Stack
    PUSHS,
    POPS,
    CLEARS,

    // Stack aritmetic instructions
    ADDS,
    SUBS,
    MULS,
    DIVS,
    IDIVS,

    // Relational instructions
    LTS,
    GTS,
    EQS,
    NEQS,
    LSES,
    GTES,
} instruction_type;

int dArray_init(dArray* array); // Alocating first count_of_scopes, and set default values
                                // Return 0 if the task was succesfull
                                // otherwise return 1

int dArray_add_to_scope(dArray* array, int index); // Increment count_in_scope[index] by 1
                                                   // Return 0 if the task was succesfull
                                                   // otherwise return 1

void dArray_despose(dArray* array); // Free memory of array
// -------------------------------------------------------------------------------------------

// Generator of 3AC --------------------------------------------------------------------------
#define MAX_DIGITS_OF_SCOPE 48

// Initiation of generator
int generator_init();
int generate_build_in_function();
void generator_print_output();

// Cleaning inner string
void generator_clear();

// Main
int genetate_main_start();
int genetate_main_end();

// Generating stack operations
int generate_add_var_to_stack(int scope, char* name_of_var);
int generate_add_string_to_stack(char* value);
int generate_add_int_to_stack(int value);
int generate_add_float_to_stack(double value);

int generate_stack_operation(instruction_type operation);

// Generating relations
int generate_relation(instruction_type relation);

// Generating for variables
int generate_new_var(int scope, char* name_of_var);
int generate_assign_var(int scope, char* name_of_var);

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
int generate_if_start(int scope, char* name_of_function);
int generate_if_else(int scope, char* name_of_function);
int generate_if_end(int scope, char* name_of_function);
// -- Generating body of for loop
int generate_for_start(int scope, char* name_of_function);
int generate_for_assignment_end(int scope, char* name_of_function);
int generate_for_comperation(int scope, char* name_of_function);
int generate_for_end(int scope, char* name_of_function);
// -- Generating body of function
int generate_function_start(char* name_of_function);
int generate_function_param(int scope, char* name_of_parameter);
int generate_function_end();

// Calling function
int generate_function_call(char* function_name);
int generate_function_return_var(int scope, char* name_of_var);

// -------------------------------------------------------------------------------------------

#endif