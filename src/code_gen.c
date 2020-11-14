/****
 * Implementace překladače imperativního jazyka IFJ20.
 * Tomáš Beneš - xbenes55
 * Generator of output code
*/

#include <stdio.h>
#include "headers/code_gen.h"
#include "headers/error_codes.h"
#include "headers/symtable.h"

// Implementation of dynemic array ----------------------------------------------------------
int dArray_init(dArray* array){
    if((array->count_in_scope=malloc(ALLOCATE_CHUNK*sizeof(int)))==NULL)
        return 1;                                                   // ERROR
    array->size_of_array=ALLOCATE_CHUNK;
    return 0;                                                       // SUCCES
}

int dArray_add_to_scope(dArray* array, int index){
    if(index>=array->size_of_array){                                // The array at index is not allocated
        int* tmp_count_in_scope;
        if((tmp_count_in_scope=realloc(array->count_in_scope,(ALLOCATE_CHUNK+array->size_of_array)*sizeof(int)))==NULL){
            free(array->count_in_scope);
            array->count_in_scope=NULL;
            return 1;                                               // ERROR
        }else{
            array->count_in_scope=tmp_count_in_scope;
            array->size_of_array+=8;
        }
    }
    array->count_in_scope[index]++;
    return 0;                                                       // SUCCES
}
// -------------------------------------------------------------------------------------------

// Implementation of Generator ---------------------------------------------------------------
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

int generate_function_start(char *function_name){
    if( strAddConstStr(&output,"LABEL ")==STR_ERROR         || \
        strAddConstStr(&output,function_name)==STR_ERROR    || \
        strAddConstStr(&output,"\n PUSHFRAME")==STR_ERROR){
            return INTERNAL_COMPILER_ERR;
        }

        // TODO vytvořit proměnné pro parametry funkce na LF.
    return OK;
}

int generate_function_end(char *function_name){
    // TODO vymyslet jak vracet návratové hodnoty
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



// Generate instruction -> "DEFVAR LF@(name of variable)\n" 
int generate_if_start(/*expr*/){   
    
    if( strAddConstStr(&output,"PUSHFRAME\nCREATEFRAME\n")==STR_ERROR){
        return INTERNAL_COMPILER_ERR;
    }
    return OK;
}

int generate_exp_if(){
     // TODO zjistit pomocí čeho identifikovat if labely
    return OK;
}

int generate_if_else(){
     
    if( strAddConstStr(&output,"JUMP if_")==STR_ERROR   || \
        strAddConstStr(&output,"a"/*něco podle čeho bude jasné jaký if to bude--->hloubka zanoření+název funkce ve které je?*/)==STR_ERROR || \
        strAddConstStr(&output,"_end\nLABEL if_")==STR_ERROR ||\
        strAddConstStr(&output,"a"/*něco podle čeho bude jasné jaký if to bude--->hloubka zanoření+název funkce ve které je?*/)==STR_ERROR || \
        strAddConstStr(&output,"_else\n")==STR_ERROR){
        return INTERNAL_COMPILER_ERR;
    }
    return OK;
}


int generate__for_if(){
    // TODO dodělat generování exp
    // TODO zjistit pomocí čeho identifikovat if labely¨
    return OK;
}

// -------------------------------------------------------------------------------------------