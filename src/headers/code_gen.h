/****
 * Implementace překladače imperativního jazyka IFJ20.
 * Tomáš Beneš - xbenes55
 * Generator of output code
*/
#include "str.h"

#ifndef code_gen_h
#define code_gen_h

// Initiation of generator
int generator_init();
int generate_build_in_function(string *output);              
void generator_print_output();

// Main
int genetate_main_start();
int genetate_main_end();        //<-- návěští pro předčasné ukončení mainu + return mainu


// Generating for variables
int generate_new_var(assignType);
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
// - prefix
int generate_start_for();
int generate_start_while();
int generate_start_if();
int generate_start_else();
int generate_start_function();
// - postfix
int generate_end_for();
int generate_end_while();
int generate_end_if();
int generate_end_else();
int generate_end_function();

// Calling function
int generate_call_function(char *function_name);
int generate_return_function();

// Cleaning inner string
void generator_clear();

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