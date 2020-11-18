/****
 * Implementace překladače imperativního jazyka IFJ20.
 * Tomáš Beneš - xbenes55
 * Generator of output code
*/

#include <stdio.h>
#include "code_gen.h"
#include "error_codes.h"
#include "symtable.h"

// Implementation of dynemic array ----------------------------------------------------------
int dArray_init(dArray* array){
    if((array->count_in_scope=malloc(ALLOCATE_CHUNK*sizeof(int)))==NULL)
        return 1;                                                   // ERROR
    array->size_of_array=ALLOCATE_CHUNK;
    for (int i = 0; i < array->size_of_array; i++)
    {
        array->count_in_scope[i]=0;
    }
    
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
            for (int i = array->size_of_array; i < array->size_of_array+ALLOCATE_CHUNK; i++)
            {
                array->count_in_scope[i]=0;
            }
            array->size_of_array+=ALLOCATE_CHUNK;
        }
    }
    array->count_in_scope[index]++;

    return 0;                                                       // SUCCES
}

void dArray_despose(dArray* array){
    free(array->count_in_scope);
}
// -------------------------------------------------------------------------------------------

// Implementation of Generator ---------------------------------------------------------------
static string output;

static dArray if_counter;
static dArray while_counter;
static dArray for_counter;



//TODO dodělat implementaci build-in funkcí
/*int generate_inputs();
int generate_inputi();
int generate_inputf();
int generate_print();
int generate_int2float();
int generate_float2int();
int generate_len();
int generate_substr();
int generate_ord();
int generate_chr();*/

int generator_init(){
    // Initiation of counters for LABLE creation
    if(dArray_init(&if_counter)==1&&dArray_init(&while_counter)==1&&dArray_init(&for_counter)==1){
        return INTERNAL_COMPILER_ERR;
    }
    // Initiation of output string
    if(strInit(&output)==STR_ERROR){
        return INTERNAL_COMPILER_ERR;
    }
    // Add prefix to the output string
    if(strAddConstStr(&output,".IFJcode20\nGF@expr\nGF@tmp\nJUMP main\n")==STR_ERROR){
        return INTERNAL_COMPILER_ERR;
    }
    // Add build-in functions to the output string
    /*if (generate_build_in_function(&output)==INTERNAL_COMPILER_ERR){
        return INTERNAL_COMPILER_ERR;
    }*/
    return OK;
}

void generator_clear(){
    dArray_despose(&if_counter);
    dArray_despose(&while_counter);
    dArray_despose(&for_counter);
    strFree(&output);
}
void generator_print_output(){
    printf("%s",output);
}

/*int generate_build_in_function(string *output){
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
}*/

// -------------------------------------------------------------------------------------------

// Generationg stack operations -----------------------------------------------------

int generate_add_var_to_stack(char *name_of_variable){
    if( strAddConstStr(&output,"PUSHS TF@")==STR_ERROR      ||\
        strAddConstStr(&output,name_of_variable)==STR_ERROR ||\
        strAddConstStr(&output,"\n")==STR_ERROR){
        return INTERNAL_COMPILER_ERR;
    }
    return OK;
}

int generate_add_const_to_stack(char *value, varType type){
    switch (type)
    {
    case INT:
        if( strAddConstStr(&output,"PUSHS int@")==STR_ERROR  ||\
            strAddConstStr(&output,value)==STR_ERROR            ||\
            strAddConstStr(&output,"\n")==STR_ERROR){
            return INTERNAL_COMPILER_ERR;
        }
        break;
    case STRING:
        if( strAddConstStr(&output,"PUSHS string@")==STR_ERROR  ||\
            strAddConstStr(&output,value)==STR_ERROR            ||\
            strAddConstStr(&output,"\n")==STR_ERROR){
            return INTERNAL_COMPILER_ERR;
        }
        break;    
    case FLOAT:
    // TODO - dodělat implementaci float
        if( strAddConstStr(&output,"PUSHS float@")==STR_ERROR  ||\
            strAddConstStr(&output,value)==STR_ERROR            ||\
            strAddConstStr(&output,"\n")==STR_ERROR){
            return INTERNAL_COMPILER_ERR;
        }
        break;    
    default:
        return INTERNAL_COMPILER_ERR;
    }
    return OK;
}

int generate_stack_operation(enum instruction_type operation){
    switch (operation)
    {
    case ADDS:
        if( strAddConstStr(&output,"ADDS\n")==STR_ERROR){
            return INTERNAL_COMPILER_ERR;
        }
        break;
    case SUBS:
        if( strAddConstStr(&output,"SUBS\n")==STR_ERROR){
            return INTERNAL_COMPILER_ERR;
        }
        break;
    case MULS:
        if( strAddConstStr(&output,"MULS\n")==STR_ERROR){
            return INTERNAL_COMPILER_ERR;
        }
        break;
    case DIVS:
        if( strAddConstStr(&output,"DIVS\n")==STR_ERROR){
            return INTERNAL_COMPILER_ERR;
        }
        break;
    case IDIVS:
        if( strAddConstStr(&output,"IDIVS\n")==STR_ERROR){
            return INTERNAL_COMPILER_ERR;
        }
        break;
    default:
        return INTERNAL_COMPILER_ERR;
        break;
    }

    return OK;
}

// -------------------------------------------------------------------------------------------

// Generating relations ----------------------------------------------------------------------

int generate_relation(enum instruction_type relation){
    switch (relation)
    {
    case LTS:
        if( strAddConstStr(&output,"LTS\nPOPS GF@tmp\n")==STR_ERROR){
            return INTERNAL_COMPILER_ERR;
        }
        break;
    case GTS:
        if( strAddConstStr(&output,"GTS\nPOPS GF@tmp\n")==STR_ERROR){
            return INTERNAL_COMPILER_ERR;
        }
        break;
    case EQS:
        if( strAddConstStr(&output,"EQS\nPOPS GF@tmp\n")==STR_ERROR){
            return INTERNAL_COMPILER_ERR;
        }
        break;
    case LSES:
        if( strAddConstStr(&output,"GTS\nNOTS\nPOPS GF@tmp\n")==STR_ERROR){
            return INTERNAL_COMPILER_ERR;
        }
        break;
    case GTES:
        if( strAddConstStr(&output,"LTS\nNOTS\nPOPS GF@tmp\n")==STR_ERROR){
            return INTERNAL_COMPILER_ERR;
        }
        break;
    default:
        return INTERNAL_COMPILER_ERR;
        break;
    }

    return OK;
}

// -------------------------------------------------------------------------------------------

// Generating for main function --------------------------------------------------------------

int genetate_main_start(){
    if( strAddConstStr(&output,"LABEL main\nCREATEFRAME\n")==STR_ERROR){
        return INTERNAL_COMPILER_ERR;
    }

    return OK;
}

int genetate_main_end(){
    if( strAddConstStr(&output,"LABEL end_of_main\nCLEARS\n")==STR_ERROR){
        return INTERNAL_COMPILER_ERR;
    }

    return OK;
} 

// -------------------------------------------------------------------------------------------

// Generating for if statement ---------------------------------------------------------------

int generate_if_start(int scope, char *name_of_function){   
    char scope_string[MAX_DIGITS_OF_SCOPE];
    char cnt_if_in_scope_string[MAX_DIGITS_OF_SCOPE];
    dArray_add_to_scope(&if_counter,scope);

    if(sprintf(scope_string, "%d", scope)<0 || sprintf(cnt_if_in_scope_string, "%d", if_counter.count_in_scope[scope])<0)
        return INTERNAL_COMPILER_ERR;

    if( strAddConstStr(&output,"JUMPIFNEQ _if_")==STR_ERROR ||\
        strAddConstStr(&output,name_of_function)==STR_ERROR                         ||\
        strAddConstStr(&output,"_")==STR_ERROR                                      ||\
        strAddConstStr(&output,scope_string)==STR_ERROR                             ||\
        strAddConstStr(&output,"_")==STR_ERROR                                      ||\
        strAddConstStr(&output,cnt_if_in_scope_string)==STR_ERROR                   ||\
        strAddConstStr(&output,"_else ")==STR_ERROR                                 ||\
        strAddConstStr(&output,"GF@expr bool@true\n")==STR_ERROR  
        ){
        return INTERNAL_COMPILER_ERR;
    }

    return OK;
}

int generate_if_else(int scope, char *name_of_function){
    char scope_string[MAX_DIGITS_OF_SCOPE];
    char cnt_if_in_scope_string[MAX_DIGITS_OF_SCOPE];

    if(sprintf(scope_string, "%d", scope)<0 || sprintf(cnt_if_in_scope_string, "%d", if_counter.count_in_scope[scope])<0)
        return INTERNAL_COMPILER_ERR;
     
    if( strAddConstStr(&output,"JUMP _if_")==STR_ERROR                              ||\
        strAddConstStr(&output,name_of_function)==STR_ERROR                         ||\
        strAddConstStr(&output,"_")==STR_ERROR                                      ||\
        strAddConstStr(&output,scope_string)==STR_ERROR                             ||\
        strAddConstStr(&output,"_")==STR_ERROR                                      ||\
        strAddConstStr(&output,cnt_if_in_scope_string)==STR_ERROR                   ||\
        strAddConstStr(&output,"_end\nLABEL _if_")==STR_ERROR                       ||\
        strAddConstStr(&output,name_of_function)==STR_ERROR                         ||\
        strAddConstStr(&output,"_")==STR_ERROR                                      ||\
        strAddConstStr(&output,scope_string)==STR_ERROR                             ||\
        strAddConstStr(&output,"_")==STR_ERROR                                      ||\
        strAddConstStr(&output,cnt_if_in_scope_string)==STR_ERROR                   ||\
        strAddConstStr(&output,"_else\n")==STR_ERROR){
        return INTERNAL_COMPILER_ERR;
    }

    return OK;
}

int generate_if_end(int scope, char *name_of_function){
    char scope_string[MAX_DIGITS_OF_SCOPE];
    char cnt_if_in_scope_string[MAX_DIGITS_OF_SCOPE];

    if(sprintf(scope_string, "%d", scope)<0 || sprintf(cnt_if_in_scope_string, "%d", if_counter.count_in_scope[scope])<0)
        return INTERNAL_COMPILER_ERR;

    if( strAddConstStr(&output,"LABEL _if_")==STR_ERROR                             ||\
        strAddConstStr(&output,name_of_function)==STR_ERROR                         ||\
        strAddConstStr(&output,"_")==STR_ERROR                                      ||\
        strAddConstStr(&output,scope_string)==STR_ERROR                             ||\
        strAddConstStr(&output,"_")==STR_ERROR                                      ||\
        strAddConstStr(&output,cnt_if_in_scope_string)==STR_ERROR                   ||\
        strAddConstStr(&output,"_end\n")==STR_ERROR){
        return INTERNAL_COMPILER_ERR;
    }

    return OK;
}

// -------------------------------------------------------------------------------------------

// Generating for while cycle ----------------------------------------------------------------

int generate_while_start(int scope, char *name_of_function){  
    char scope_string[MAX_DIGITS_OF_SCOPE];
    char cnt_while_in_scope_string[MAX_DIGITS_OF_SCOPE];    
    dArray_add_to_scope(&while_counter,scope);

    if(sprintf(scope_string, "%d", scope)<0 || sprintf(cnt_while_in_scope_string, "%d", while_counter.count_in_scope[scope])<0)
        return INTERNAL_COMPILER_ERR;

    if( strAddConstStr(&output,"LABEL _while_")==STR_ERROR                          ||\
        strAddConstStr(&output,name_of_function)==STR_ERROR                         ||\
        strAddConstStr(&output,"_")==STR_ERROR                                      ||\
        strAddConstStr(&output,scope_string)==STR_ERROR                             ||\
        strAddConstStr(&output,"_")==STR_ERROR                                      ||\
        strAddConstStr(&output,cnt_while_in_scope_string)==STR_ERROR                ||\
        strAddConstStr(&output,"_start\n")==STR_ERROR  ){
        return INTERNAL_COMPILER_ERR;
    }
    return OK;
}


int generate_while_relation_end(int scope, char *name_of_function){
    char scope_string[MAX_DIGITS_OF_SCOPE];
    char cnt_while_in_scope_string[MAX_DIGITS_OF_SCOPE];


    if(sprintf(scope_string, "%d", scope)<0 || sprintf(cnt_while_in_scope_string, "%d", while_counter.count_in_scope[scope])<0)
        return INTERNAL_COMPILER_ERR;

    if( strAddConstStr(&output,"JUMPIFNEQ _while_")==STR_ERROR                      ||\
        strAddConstStr(&output,name_of_function)==STR_ERROR                         ||\
        strAddConstStr(&output,"_")==STR_ERROR                                      ||\
        strAddConstStr(&output,scope_string)==STR_ERROR                             ||\
        strAddConstStr(&output,"_")==STR_ERROR                                      ||\
        strAddConstStr(&output,cnt_while_in_scope_string)==STR_ERROR                ||\
        strAddConstStr(&output,"_end ")==STR_ERROR                                  ||\
        strAddConstStr(&output,"GF@expr bool@true\n")==STR_ERROR  
        ){
        return INTERNAL_COMPILER_ERR;
    }
    return OK;
}

int generate_while_end(int scope, char *name_of_function){
    char scope_string[MAX_DIGITS_OF_SCOPE];
    char cnt_while_in_scope_string[MAX_DIGITS_OF_SCOPE];

    if(sprintf(scope_string, "%d", scope)<0 || sprintf(cnt_while_in_scope_string, "%d", while_counter.count_in_scope[scope])<0)
        return INTERNAL_COMPILER_ERR;

    if( strAddConstStr(&output,"JUMP _while_")==STR_ERROR                           ||\
        strAddConstStr(&output,name_of_function)==STR_ERROR                         ||\
        strAddConstStr(&output,"_")==STR_ERROR                                      ||\
        strAddConstStr(&output,scope_string)==STR_ERROR                             ||\
        strAddConstStr(&output,"_")==STR_ERROR                                      ||\
        strAddConstStr(&output,cnt_while_in_scope_string)==STR_ERROR                ||\
        strAddConstStr(&output,"_start\n")                                          ||\
        strAddConstStr(&output,"LABEL _while_")==STR_ERROR                          ||\
        strAddConstStr(&output,name_of_function)==STR_ERROR                         ||\
        strAddConstStr(&output,"_")==STR_ERROR                                      ||\
        strAddConstStr(&output,scope_string)==STR_ERROR                             ||\
        strAddConstStr(&output,"_")==STR_ERROR                                      ||\
        strAddConstStr(&output,cnt_while_in_scope_string)==STR_ERROR                ||\
        strAddConstStr(&output,"_end\n")==STR_ERROR){
        return INTERNAL_COMPILER_ERR;
    }

    return OK;
}

// -------------------------------------------------------------------------------------------
/*











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
}*/

// -------------------------------------------------------------------------------------------