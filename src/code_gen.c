/****
 * Implementace překladače imperativního jazyka IFJ20.
 * Tomáš Beneš - xbenes55
 * Generator of output code
*/

#include <stdio.h>
#include "headers/code_gen.h"
#include "headers/error_codes.h"
#include "headers/symtable.h"

static string output;

//TODO dodělat implementaci build-in funkcí
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

int generator_init(){
    // Initiation of output string
    if(strInit(&output)==STR_ERROR){
        return INTERNAL_COMPILER_ERR;
    }
    // Add prefix to the output string
    if(strAddConstStr(&output,"./IFJcode20\n")==STR_ERROR){
        return INTERNAL_COMPILER_ERR;
    }
    // Add build-in functions to the output string
    if (generate_build_in_function(&output)==INTERNAL_COMPILER_ERR){
        return INTERNAL_COMPILER_ERR;
    }
    return OK;
}

void generator_clear(){
    strFree(&output);
}

int generate_build_in_function(string *output){
    if( generate_inputs()!=OK       || \
        generate_inputi()!=OK       || \
        generate_inputf()!=OK       || \
        generate_print()!=OK        || \
        generate_int2float()!=OK    || \
        generate_float2int()!=OK    || \
        generate_len()!=OK          || \
        generate_substr()!=OK       || \
        generate_ord()!=OK          || \
        generate_chr()!=OK){
            return INTERNAL_COMPILER_ERR;
        }
        return OK;
}

int genetate_main_start(){
    if( strAddConstStr(&output,"LABEL main\nCREATEFRAME\nPUSHFRAME")==STR_ERROR){
        return INTERNAL_COMPILER_ERR;
    }
    return OK;
}
int genetate_main_end(){
    if( strAddConstStr(&output,"LABEL end_of_main\nPOPFRAME\nCLEARS")==STR_ERROR){
        return INTERNAL_COMPILER_ERR;
    }
    return OK;
} 

int generate_start_function(char *function_name){
    if( strAddConstStr(&output,"LABEL ")==STR_ERROR         || \
        strAddConstStr(&output,function_name)==STR_ERROR    || \
        strAddConstStr(&output,"\n PUSHFRAME")==STR_ERROR){
            return INTERNAL_COMPILER_ERR;
        }
    return OK;
}

int generate_end_function(char *function_name){
    if( strAddConstStr(&output,"LABEL end_of_")==STR_ERROR  || \
        strAddConstStr(&output,function_name)==STR_ERROR    || \
        strAddConstStr(&output,"\nPOPFRAME\nRETURN\n")==STR_ERROR){
            return INTERNAL_COMPILER_ERR;
        }
    return OK;
}

// Generate instruction -> "DEFVAR LF@(name of variable)\n" 
// TODO zjistit jak chce víťa volat vytváření funkcí
int generate_new_var(char *var_name){   
    if( strAddConstStr(&output,"DEFVAR LF@")==STR_ERROR||\
        strAddConstStr(&output,var_name)==STR_ERROR||\
        strAddConstStr(&output,"\n")==STR_ERROR){

        return INTERNAL_COMPILER_ERR;
    }
    return OK;
}
