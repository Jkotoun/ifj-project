/****
 * Implementace překladače imperativního jazyka IFJ20.
 * Tomáš Beneš - xbenes55
 * Generator of output code
*/

#include "code_gen.h"
#include "error_codes.h"
#include "symtable.h"
#include <stdio.h>
#include <string.h>

// Implementation of dynemic array ----------------------------------------------------------
int dArray_init(dArray* array)
{
    if ((array->count_in_scope = malloc(ALLOCATE_CHUNK * sizeof(int))) == NULL)
        return 1; // ERROR
    array->size_of_array = ALLOCATE_CHUNK;
    for (int i = 0; i < array->size_of_array; i++)
    {
        array->count_in_scope[i] = 0;
    }

    return 0; // SUCCES
}

int dArray_add_to_scope(dArray* array, int index)
{
    if (index >= array->size_of_array)
    { // The array at index is not allocated
        int* tmp_count_in_scope;
        if ((tmp_count_in_scope = realloc(array->count_in_scope, (ALLOCATE_CHUNK + array->size_of_array) * sizeof(int))) == NULL)
        {
            free(array->count_in_scope);
            array->count_in_scope = NULL;
            return 1; // ERROR
        }
        else
        {
            array->count_in_scope = tmp_count_in_scope;
            for (int i = array->size_of_array; i < array->size_of_array + ALLOCATE_CHUNK; i++)
            {
                array->count_in_scope[i] = 0;
            }
            array->size_of_array += ALLOCATE_CHUNK;
        }
    }
    array->count_in_scope[index]++;

    return 0; // SUCCES
}

void dArray_despose(dArray* array)
{
    free(array->count_in_scope);
}
// -------------------------------------------------------------------------------------------

// Implementation of Generator ---------------------------------------------------------------
static string output;

static dArray if_counter;
static dArray for_counter;

//func len(𝑠 string) (int) – Vrátí délku (počet znaků) řetězce zadaného jediným parametrem 𝑠. Např. len("x\nz") vrací 3.
int generate_len()
{
    // Creates parameter variables
    if (strAddConstStr(&output, "\
LABEL len\n\
PUSHFRAME\n\
CREATEFRAME\n\
POPS GF@trash\n\
DEFVAR TF@str\n\
POPS TF@str\n\
DEFVAR TF@cnt\n\
MOVE TF@cnt int@0\n\
STRLEN TF@cnt TF@str\n\
PUSHS TF@cnt\n\
POPFRAME\n\
                                RETURN\n")==STR_ERROR)
        return INTERNAL_COMPILER_ERR;
    return OK;
}

int generate_int2float(){
    if(strAddConstStr(&output, "\
LABEL int2float\n\
POPS GF@trash\n\
INT2FLOATS\n\
                                RETURN\n")==STR_ERROR)
        return INTERNAL_COMPILER_ERR;
    return OK;
}

int generate_float2int(){
    if(strAddConstStr(&output, "\
LABEL float2int\n\
POPS GF@trash\n\
FLOAT2INTS\n\
                                RETURN\n")==STR_ERROR)
    return INTERNAL_COMPILER_ERR;
    return OK;
}

int generate_print(){
    if(strAddConstStr(&output, "\
LABEL print\n\
PUSHFRAME\n\
CREATEFRAME\n\
DEFVAR TF@cnt_of_parameter\n\
DEFVAR TF@to_print\n\
DEFVAR TF@cnt\n\
POPS TF@cnt_of_parameter\n\
MOVE TF@cnt int@0\n\
LABEL _print_while_start\n\
LT GF@expr TF@cnt TF@cnt_of_parameter\n\
JUMPIFNEQ _print_while_end GF@expr bool@true\n\
POPS TF@to_print\n\
WRITE TF@to_print\n\
ADD TF@cnt TF@cnt int@1\n\
JUMP _print_while_start\n\
LABEL _print_while_end\n\
POPFRAME\n\
RETURN\n")==STR_ERROR)
    return INTERNAL_COMPILER_ERR;
    return OK;
}

int generate_inputs(){
    if(strAddConstStr(&output, "\
LABEL inputs\n\
PUSHFRAME\n\
CREATEFRAME\n\
POPS GF@trash\n\
DEFVAR TF@out_string\n\
READ TF@out_string string\n\
PUSHS TF@out_string\n\
PUSHS int@0\n\
POPFRAME\n\
RETURN\n")==STR_ERROR)
        return INTERNAL_COMPILER_ERR;
    return OK;
}


int generate_ord(){
        if(strAddConstStr(&output, "\
LABEL ord\n\
PUSHFRAME\n\
CREATEFRAME\n\
POPS GF@trash\n\
DEFVAR TF@ascii\n\
DEFVAR TF@err\n\
MOVE TF@ascii string@\n\
MOVE TF@err int@1\n\
DEFVAR TF@l_limit\n\
DEFVAR TF@r_limit\n\
DEFVAR TF@string\n\
DEFVAR TF@i\n\
POPS TF@string\n\
POPS TF@i\n\
DEFVAR TF@str_len\n\
MOVE TF@str_len int@0\n\
STRLEN TF@str_len TF@string\n\
SUB TF@str_len TF@str_len int@1\n\
LT TF@l_limit TF@i int@0\n\
NOT TF@l_limit TF@l_limit\n\
GT TF@r_limit TF@i TF@str_len\n\
NOT TF@r_limit TF@r_limit\n\
AND TF@l_limit TF@l_limit TF@r_limit\n\
JUMPIFNEQ _ord_end TF@l_limit bool@true\n\
MOVE TF@err int@0\n\
STRI2INT TF@ascii TF@string TF@i\n\
LABEL _ord_end\n\
PUSHS TF@ascii\n\
PUSHS TF@err\n\
POPFRAME\n\
RETURN\n")==STR_ERROR)
        return INTERNAL_COMPILER_ERR;
    return OK;
}

int generate_chr(){
        if(strAddConstStr(&output, "\
LABEL chr\n\
PUSHFRAME\n\
CREATEFRAME\n\
POPS GF@trash\n\
DEFVAR TF@ret_str\n\
MOVE TF@ret_str string@\n\
DEFVAR TF@err\n\
MOVE TF@err int@1\n\
DEFVAR TF@l_limit\n\
DEFVAR TF@r_limit\n\
DEFVAR TF@i\n\
POPS TF@i\n\
LT TF@l_limit TF@i int@0\n\
NOT TF@l_limit TF@l_limit\n\
GT TF@r_limit TF@i int@255\n\
NOT TF@r_limit TF@r_limit\n\
AND TF@l_limit TF@l_limit TF@r_limit\n\
JUMPIFNEQ _chr_end TF@l_limit bool@true\n\
MOVE TF@err int@0\n\
INT2CHAR TF@ret_str TF@i\n\
LABEL _chr_end\n\
PUSHS TF@ret_str\n\
PUSHS TF@err\n\
POPFRAME\n\
RETURN\n")==STR_ERROR)
        return INTERNAL_COMPILER_ERR;
    return OK;
}

int generate_substr(){
        if(strAddConstStr(&output, "\
LABEL substr\n\
PUSHFRAME\n\
CREATEFRAME\n\
POPS GF@trash\n\
DEFVAR TF@ret_str\n\
DEFVAR TF@err\n\
MOVE TF@ret_str string@\n\
MOVE TF@err int@1\n\
DEFVAR TF@s\n\
POPS TF@s\n\
DEFVAR TF@i\n\
POPS TF@i\n\
DEFVAR TF@n\n\
POPS TF@n\n\
DEFVAR TF@char\n\
MOVE TF@char string@\n\
DEFVAR TF@str_len\n\
DEFVAR TF@l_limit\n\
DEFVAR TF@r_limit\n\
STRLEN TF@str_len TF@s\n\
LT TF@l_limit TF@i int@0\n\
NOT TF@l_limit TF@l_limit\n\
GT TF@r_limit TF@i TF@str_len\n\
NOT TF@r_limit TF@r_limit\n\
AND TF@l_limit TF@l_limit TF@r_limit\n\
LT TF@r_limit TF@n int@0\n\
NOT TF@r_limit TF@r_limit\n\
AND TF@l_limit TF@l_limit TF@r_limit\n\
JUMPIFNEQ _sub_end TF@l_limit bool@true\n\
MOVE TF@err int@0\n\
DEFVAR TF@cnt_of_loaded\n\
MOVE TF@cnt_of_loaded int@0\n\
LABEL _sub_while\n\
LT TF@l_limit TF@i TF@str_len\n\
LT TF@r_limit TF@cnt_of_loaded TF@n \n\
AND TF@l_limit TF@l_limit TF@r_limit\n\
JUMPIFNEQ _sub_end TF@l_limit bool@true\n\
GETCHAR TF@char TF@s TF@i\n\
CONCAT TF@ret_str TF@ret_str TF@char\n\
ADD TF@cnt_of_loaded TF@cnt_of_loaded int@1\n\
ADD TF@i TF@i int@1\n\
JUMP _sub_while\n\
LABEL _sub_end\n\
PUSHS TF@ret_str\n\
PUSHS TF@err\n\
POPFRAME\n\
RETURN\n")==STR_ERROR)
        return INTERNAL_COMPILER_ERR;
    return OK;
}


//TODO dodělat implementaci build-in funkcí
/*
int generate_inputi();
int generate_inputf();
*/
int generator_init()
{
    // Initiation of counters for LABLE creation
    if (dArray_init(&if_counter) == 1 && dArray_init(&for_counter) == 1)
    {
        return INTERNAL_COMPILER_ERR;
    }
    // Initiation of output string
    if (strInit(&output) == STR_ERROR)
    {
        return INTERNAL_COMPILER_ERR;
    }
    // Add prefix to the output string
    if(strAddConstStr(&output,".IFJcode20\nDEFVAR GF@expr\nDEFVAR GF@tmp\nDEFVAR GF@trash\nDEFVAR GF@concat_l\nDEFVAR GF@concat_r\nJUMP main\nLABEL _div_0\nEXIT int@9\n")==STR_ERROR){
        return INTERNAL_COMPILER_ERR;
    }
    // Add build-in functions to the output string
    if (generate_build_in_function()==INTERNAL_COMPILER_ERR){
        return INTERNAL_COMPILER_ERR;
    }
    return OK;
}

void generator_clear()
{
    dArray_despose(&if_counter);
    dArray_despose(&for_counter);
    strFree(&output);
}

void generator_print_output(){
    printf("%s",output.str);
}

int generate_build_in_function(){
    if( generate_inputs()!=OK       || 
        //generate_inputi()!=OK       || 
        //generate_inputf()!=OK       || 
        generate_chr()!=OK        || 
        generate_int2float()!=OK    || 
        generate_float2int()!=OK    || 
        generate_len()!=OK          || 
        generate_substr()!=OK       || 
        generate_ord()!=OK          || 
        generate_print()!=OK){
            return INTERNAL_COMPILER_ERR;
        }
        return OK;
}

// -------------------------------------------------------------------------------------------

// Generationg stack operations --------------------------------------------------------------
int generate_add_concat_to_stack(){
    if( strAddConstStr(&output,"\
POPS GF@concat_l\n\
                                POPS GF@concat_r\n\
                                CONCAT GF@concat_l GF@concat_r GF@concat_l\n\
                                PUSHS GF@concat_l\n")==STR_ERROR){
            return INTERNAL_COMPILER_ERR;
        }
    return OK;
}

int generate_add_var_to_stack(int scope, char* name_of_var)
{
    char scope_string[MAX_DIGITS_OF_SCOPE];

    if (sprintf(scope_string, "%d", scope) < 0)
        return INTERNAL_COMPILER_ERR;

    if (strAddConstStr(&output, "PUSHS TF@") == STR_ERROR ||
        strAddConstStr(&output, name_of_var) == STR_ERROR ||
        strAddConstStr(&output, "_") == STR_ERROR ||
        strAddConstStr(&output, scope_string) == STR_ERROR ||
        strAddConstStr(&output, "\n") == STR_ERROR)
    {
        return INTERNAL_COMPILER_ERR;
    }
    return OK;
}

int generate_add_string_to_stack(char *value){
    int index=0;
    if( strAddConstStr(&output,"PUSHS string@")==STR_ERROR)
        return INTERNAL_COMPILER_ERR;
    while(value[index]!='\0'){
        if(((int)value[index]>=0&&(int)value[index]<=32)||(int)value[index]==35||(int)value[index]==92){
            char ascii_string[MAX_DIGITS_OF_SCOPE];
            if(sprintf(ascii_string, "%03d", (int)value[index])<0)
                return INTERNAL_COMPILER_ERR;
            if( strAddConstStr(&output,"\\")==STR_ERROR ||
                strAddConstStr(&output,ascii_string)==STR_ERROR)
                return INTERNAL_COMPILER_ERR;
        }else{
            if(strAddChar(&output, value[index])==STR_ERROR)
                    return INTERNAL_COMPILER_ERR;
        }

        index++;
    }
    if (strAddConstStr(&output, "\n") == STR_ERROR)
        return INTERNAL_COMPILER_ERR;

    return OK;
}
int generate_add_int_to_stack(int value)
{
    char value_string[MAX_DIGITS_OF_SCOPE];

    if (sprintf(value_string, "%d", value) < 0)
        return INTERNAL_COMPILER_ERR;

    if (strAddConstStr(&output, "PUSHS int@") == STR_ERROR ||
        strAddConstStr(&output, value_string) == STR_ERROR ||
        strAddConstStr(&output, "\n") == STR_ERROR)
    {
        return INTERNAL_COMPILER_ERR;
    }
    return OK;
}
int generate_add_float_to_stack(double value)
{
    char value_string[MAX_DIGITS_OF_SCOPE];

    if (sprintf(value_string, "%a", value) < 0)
        return INTERNAL_COMPILER_ERR;

    if (strAddConstStr(&output, "PUSHS float@") == STR_ERROR ||
        strAddConstStr(&output, value_string) == STR_ERROR ||
        strAddConstStr(&output, "\n") == STR_ERROR)
    {
        return INTERNAL_COMPILER_ERR;
    }
    return OK;
}

int generate_stack_operation(instruction_type operation)
{
    switch (operation)
    {
    case ADDS:
        if (strAddConstStr(&output, "ADDS\n") == STR_ERROR)
        {
            return INTERNAL_COMPILER_ERR;
        }
        break;
    case SUBS:
        if (strAddConstStr(&output, "SUBS\n") == STR_ERROR)
        {
            return INTERNAL_COMPILER_ERR;
        }
        break;
    case MULS:
        if (strAddConstStr(&output, "MULS\n") == STR_ERROR)
        {
            return INTERNAL_COMPILER_ERR;
        }
        break;
    case DIVS:
        if (strAddConstStr(&output, "POPS GF@tmp\nEQ GF@expr GF@tmp float@0x0p+0\nJUMPIFEQ _div_0 GF@expr bool@true\nPUSHS GF@tmp\nDIVS\n") == STR_ERROR)
        {
            return INTERNAL_COMPILER_ERR;
        }
        break;
    case IDIVS:
        if (strAddConstStr(&output, "POPS GF@tmp\nEQ GF@expr GF@tmp int@0\nJUMPIFEQ _div_0 GF@expr bool@true\nPUSHS GF@tmp\nIDIVS\n") == STR_ERROR)
        {
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

int generate_relation(instruction_type relation)
{
    switch (relation)
    {
    case LTS:
        if (strAddConstStr(&output, "LTS\nPOPS GF@expr\n") == STR_ERROR)
        {
            return INTERNAL_COMPILER_ERR;
        }
        break;
    case GTS:
        if (strAddConstStr(&output, "GTS\nPOPS GF@expr\n") == STR_ERROR)
        {
            return INTERNAL_COMPILER_ERR;
        }
        break;
    case EQS:
        if (strAddConstStr(&output, "EQS\nPOPS GF@expr\n") == STR_ERROR)
        {
            return INTERNAL_COMPILER_ERR;
        }
        break;
    case NEQS:
        if (strAddConstStr(&output, "EQS\nNOTS\nPOPS GF@expr\n") == STR_ERROR)
        {
            return INTERNAL_COMPILER_ERR;
        }
        break;
    case LSES:
        if (strAddConstStr(&output, "GTS\nNOTS\nPOPS GF@expr\n") == STR_ERROR)
        {
            return INTERNAL_COMPILER_ERR;
        }
        break;
    case GTES:
        if (strAddConstStr(&output, "LTS\nNOTS\nPOPS GF@expr\n") == STR_ERROR)
        {
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

// Generating for variables-------------------------------------------------------------------
int generate_new_var(int scope, char* name_of_var)
{
    char scope_string[MAX_DIGITS_OF_SCOPE];

    if (sprintf(scope_string, "%d", scope) < 0)
        return INTERNAL_COMPILER_ERR;

    if (strAddConstStr(&output, "DEFVAR TF@") == STR_ERROR ||
        strAddConstStr(&output, name_of_var) == STR_ERROR ||
        strAddConstStr(&output, "_") == STR_ERROR ||
        strAddConstStr(&output, scope_string) == STR_ERROR ||
        strAddConstStr(&output, "\n") == STR_ERROR)
    {
        return INTERNAL_COMPILER_ERR;
    }
    return OK;
}
int generate_assign_var(int scope, char* name_of_var)
{
    char scope_string[MAX_DIGITS_OF_SCOPE];
    if (strcmp(name_of_var, "_") == 0)
    {
        if (strAddConstStr(&output, "POPS GF@trash\n") == STR_ERROR)
            return INTERNAL_COMPILER_ERR;
    }
    else
    {
        if (sprintf(scope_string, "%d", scope) < 0)
            return INTERNAL_COMPILER_ERR;

        if (strAddConstStr(&output, "POPS TF@") == STR_ERROR ||
            strAddConstStr(&output, name_of_var) == STR_ERROR ||
            strAddConstStr(&output, "_") == STR_ERROR ||
            strAddConstStr(&output, scope_string) == STR_ERROR ||
            strAddConstStr(&output, "\n") == STR_ERROR)
        {
            return INTERNAL_COMPILER_ERR;
        }
    }
    return OK;
}
// -------------------------------------------------------------------------------------------

// Generating for main function --------------------------------------------------------------

int generate_main_start()
{
    if (strAddConstStr(&output, "LABEL main\nCREATEFRAME\n") == STR_ERROR)
    {
        return INTERNAL_COMPILER_ERR;
    }

    return OK;
}

int generate_main_end()
{
    if (strAddConstStr(&output, "LABEL end_of_main\nCLEARS\n") == STR_ERROR)
    {
        return INTERNAL_COMPILER_ERR;
    }

    return OK;
}

// -------------------------------------------------------------------------------------------

// Generating for if statement ---------------------------------------------------------------

int generate_if_start(int scope, char* name_of_function)
{
    char scope_string[MAX_DIGITS_OF_SCOPE];
    char cnt_if_in_scope_string[MAX_DIGITS_OF_SCOPE];
    dArray_add_to_scope(&if_counter, scope);

    if (sprintf(scope_string, "%d", scope) < 0 || sprintf(cnt_if_in_scope_string, "%d", if_counter.count_in_scope[scope]) < 0)
        return INTERNAL_COMPILER_ERR;

    if (strAddConstStr(&output, "JUMPIFNEQ _if_") == STR_ERROR ||
        strAddConstStr(&output, name_of_function) == STR_ERROR ||
        strAddConstStr(&output, "_") == STR_ERROR ||
        strAddConstStr(&output, scope_string) == STR_ERROR ||
        strAddConstStr(&output, "_") == STR_ERROR ||
        strAddConstStr(&output, cnt_if_in_scope_string) == STR_ERROR ||
        strAddConstStr(&output, "_else ") == STR_ERROR ||
        strAddConstStr(&output, "GF@expr bool@true\n") == STR_ERROR)
    {
        return INTERNAL_COMPILER_ERR;
    }

    return OK;
}

int generate_if_else(int scope, char* name_of_function)
{
    char scope_string[MAX_DIGITS_OF_SCOPE];
    char cnt_if_in_scope_string[MAX_DIGITS_OF_SCOPE];

    if (sprintf(scope_string, "%d", scope) < 0 || sprintf(cnt_if_in_scope_string, "%d", if_counter.count_in_scope[scope]) < 0)
        return INTERNAL_COMPILER_ERR;

    if (strAddConstStr(&output, "JUMP _if_") == STR_ERROR ||
        strAddConstStr(&output, name_of_function) == STR_ERROR ||
        strAddConstStr(&output, "_") == STR_ERROR ||
        strAddConstStr(&output, scope_string) == STR_ERROR ||
        strAddConstStr(&output, "_") == STR_ERROR ||
        strAddConstStr(&output, cnt_if_in_scope_string) == STR_ERROR ||
        strAddConstStr(&output, "_end\nLABEL _if_") == STR_ERROR ||
        strAddConstStr(&output, name_of_function) == STR_ERROR ||
        strAddConstStr(&output, "_") == STR_ERROR ||
        strAddConstStr(&output, scope_string) == STR_ERROR ||
        strAddConstStr(&output, "_") == STR_ERROR ||
        strAddConstStr(&output, cnt_if_in_scope_string) == STR_ERROR ||
        strAddConstStr(&output, "_else\n") == STR_ERROR)
    {
        return INTERNAL_COMPILER_ERR;
    }

    return OK;
}

int generate_if_end(int scope, char* name_of_function)
{
    char scope_string[MAX_DIGITS_OF_SCOPE];
    char cnt_if_in_scope_string[MAX_DIGITS_OF_SCOPE];

    if (sprintf(scope_string, "%d", scope) < 0 || sprintf(cnt_if_in_scope_string, "%d", if_counter.count_in_scope[scope]) < 0)
        return INTERNAL_COMPILER_ERR;

    if (strAddConstStr(&output, "LABEL _if_") == STR_ERROR ||
        strAddConstStr(&output, name_of_function) == STR_ERROR ||
        strAddConstStr(&output, "_") == STR_ERROR ||
        strAddConstStr(&output, scope_string) == STR_ERROR ||
        strAddConstStr(&output, "_") == STR_ERROR ||
        strAddConstStr(&output, cnt_if_in_scope_string) == STR_ERROR ||
        strAddConstStr(&output, "_end\n") == STR_ERROR)
    {
        return INTERNAL_COMPILER_ERR;
    }

    return OK;
}

// -------------------------------------------------------------------------------------------

// Generating for for cycle ----------------------------------------------------------------

int generate_for_compare(int scope, char *name_of_function){
    char scope_string[MAX_DIGITS_OF_SCOPE];
    char cnt_for_in_scope_string[MAX_DIGITS_OF_SCOPE];

    dArray_add_to_scope(&for_counter, scope);

    if(sprintf(scope_string, "%d", scope)<0 || sprintf(cnt_for_in_scope_string, "%d", for_counter.count_in_scope[scope])<0)
        return INTERNAL_COMPILER_ERR;

    if( strAddConstStr(&output,"LABEL _for_")==STR_ERROR                          ||\
        strAddConstStr(&output,name_of_function)==STR_ERROR                         ||\
        strAddConstStr(&output,"_")==STR_ERROR                                      ||\
        strAddConstStr(&output,scope_string)==STR_ERROR                             ||\
        strAddConstStr(&output,"_")==STR_ERROR                                      ||\
        strAddConstStr(&output,cnt_for_in_scope_string)==STR_ERROR                ||\
        strAddConstStr(&output,"_compare\n")){
        return INTERNAL_COMPILER_ERR;
    }
    return OK;
}


int generate_for_assignment(int scope, char *name_of_function){
    char scope_string[MAX_DIGITS_OF_SCOPE];
    char cnt_for_in_scope_string[MAX_DIGITS_OF_SCOPE];

    if (sprintf(scope_string, "%d", scope) < 0 || sprintf(cnt_for_in_scope_string, "%d", for_counter.count_in_scope[scope]) < 0)
        return INTERNAL_COMPILER_ERR;

    if( strAddConstStr(&output,"JUMPIFNEQ _for_")==STR_ERROR                      ||\
        strAddConstStr(&output,name_of_function)==STR_ERROR                         ||\
        strAddConstStr(&output,"_")==STR_ERROR                                      ||\
        strAddConstStr(&output,scope_string)==STR_ERROR                             ||\
        strAddConstStr(&output,"_")==STR_ERROR                                      ||\
        strAddConstStr(&output,cnt_for_in_scope_string)==STR_ERROR                ||\
        strAddConstStr(&output,"_end ")==STR_ERROR                                  ||\
        strAddConstStr(&output,"GF@expr bool@true\nJUMP _for_")==STR_ERROR  ||
        strAddConstStr(&output,name_of_function)==STR_ERROR                         ||\
        strAddConstStr(&output,"_")==STR_ERROR                                      ||\
        strAddConstStr(&output,scope_string)==STR_ERROR                             ||\
        strAddConstStr(&output,"_")==STR_ERROR                                      ||\
        strAddConstStr(&output,cnt_for_in_scope_string)==STR_ERROR                ||\
        strAddConstStr(&output,"_code\nLABEL _for_")==STR_ERROR  ||
        strAddConstStr(&output,name_of_function)==STR_ERROR                         ||\
        strAddConstStr(&output,"_")==STR_ERROR                                      ||\
        strAddConstStr(&output,scope_string)==STR_ERROR                             ||\
        strAddConstStr(&output,"_")==STR_ERROR                                      ||\
        strAddConstStr(&output,cnt_for_in_scope_string)==STR_ERROR                ||\
        strAddConstStr(&output,"_assignment\n")==STR_ERROR){
        return INTERNAL_COMPILER_ERR;
    }
    return OK; 
}

int generate_for_before_code(int scope, char *name_of_function){
    char scope_string[MAX_DIGITS_OF_SCOPE];
    char cnt_for_in_scope_string[MAX_DIGITS_OF_SCOPE];

    if(sprintf(scope_string, "%d", scope)<0 || sprintf(cnt_for_in_scope_string, "%d", for_counter.count_in_scope[scope])<0)
        return INTERNAL_COMPILER_ERR;

    if( strAddConstStr(&output,"JUMP _for_")==STR_ERROR                           ||\
        strAddConstStr(&output,name_of_function)==STR_ERROR                         ||\
        strAddConstStr(&output,"_")==STR_ERROR                                      ||\
        strAddConstStr(&output,scope_string)==STR_ERROR                             ||\
        strAddConstStr(&output,"_")==STR_ERROR                                      ||\
        strAddConstStr(&output,cnt_for_in_scope_string)==STR_ERROR                ||\
        strAddConstStr(&output,"_compare\nLABEL _for_")==STR_ERROR                          ||\
        strAddConstStr(&output,name_of_function)==STR_ERROR                         ||\
        strAddConstStr(&output,"_")==STR_ERROR                                      ||\
        strAddConstStr(&output,scope_string)==STR_ERROR                             ||\
        strAddConstStr(&output,"_")==STR_ERROR                                      ||\
        strAddConstStr(&output,cnt_for_in_scope_string)==STR_ERROR                ||\
        strAddConstStr(&output,"_code\n")==STR_ERROR){
        return INTERNAL_COMPILER_ERR;
    }

    return OK;
}

int generate_for_end(int scope, char* name_of_function)
{
    char scope_string[MAX_DIGITS_OF_SCOPE];
    char cnt_for_in_scope_string[MAX_DIGITS_OF_SCOPE];

    if (sprintf(scope_string, "%d", scope) < 0 || sprintf(cnt_for_in_scope_string, "%d", for_counter.count_in_scope[scope]) < 0)
        return INTERNAL_COMPILER_ERR;

    if (strAddConstStr(&output, "JUMP _for_") == STR_ERROR ||
        strAddConstStr(&output, name_of_function) == STR_ERROR ||
        strAddConstStr(&output, "_") == STR_ERROR ||
        strAddConstStr(&output, scope_string) == STR_ERROR ||
        strAddConstStr(&output, "_") == STR_ERROR ||
        strAddConstStr(&output, cnt_for_in_scope_string) == STR_ERROR ||
        strAddConstStr(&output, "_assignment\nLABEL _for_") == STR_ERROR ||
        strAddConstStr(&output, name_of_function) == STR_ERROR ||
        strAddConstStr(&output, "_") == STR_ERROR ||
        strAddConstStr(&output, scope_string) == STR_ERROR ||
        strAddConstStr(&output, "_") == STR_ERROR ||
        strAddConstStr(&output, cnt_for_in_scope_string) == STR_ERROR ||
        strAddConstStr(&output, "_end\n") == STR_ERROR)
    {
        return INTERNAL_COMPILER_ERR;
    }

    return OK;
}

// -------------------------------------------------------------------------------------------

// Generating function -----------------------------------------------------------------------

int generate_function_start(char* name_of_function)
{

    if( strAddConstStr(&output,"LABEL ")==STR_ERROR                      ||\
        strAddConstStr(&output,name_of_function)==STR_ERROR                         ||\
        strAddConstStr(&output,"\nPUSHFRAME\nCREATEFRAME\n")==STR_ERROR||
        strAddConstStr(&output,"POPS GF@trash\n")==STR_ERROR){
        return INTERNAL_COMPILER_ERR;
    }
    return OK;
}

int generate_function_param(int scope, char* name_of_parameter)
{
    char scope_string[MAX_DIGITS_OF_SCOPE];

    if (sprintf(scope_string, "%d", scope) < 0)
        return INTERNAL_COMPILER_ERR;

    if (strAddConstStr(&output, "DEFVAR TF@") == STR_ERROR ||
        strAddConstStr(&output, name_of_parameter) == STR_ERROR ||
        strAddConstStr(&output, "_") == STR_ERROR ||
        strAddConstStr(&output, scope_string) == STR_ERROR ||
        strAddConstStr(&output, "\nPOPS TF@") == STR_ERROR ||
        strAddConstStr(&output, name_of_parameter) == STR_ERROR ||
        strAddConstStr(&output, "_") == STR_ERROR ||
        strAddConstStr(&output, scope_string) == STR_ERROR ||
        strAddConstStr(&output, "\n") == STR_ERROR)
    {
        return INTERNAL_COMPILER_ERR;
    }
    return OK;
}

int generate_function_return(char* name_of_function)
{
    if (strAddConstStr(&output, "JUMP end_of_") == STR_ERROR ||
        strAddConstStr(&output, name_of_function) == STR_ERROR ||
        strAddConstStr(&output, "\n") == STR_ERROR)
    {
        return INTERNAL_COMPILER_ERR;
    }
    return OK;
}

int generate_function_end(char* name_of_function)
{
    if (strAddConstStr(&output, "LABEL end_of_") == STR_ERROR ||
        strAddConstStr(&output, name_of_function) == STR_ERROR ||
        strAddConstStr(&output, "\nPOPFRAME\nRETURN\n") == STR_ERROR)
    {
        return INTERNAL_COMPILER_ERR;
    }
    return OK;
}

// Calling function
int generate_function_call(char *name_of_function, int number_of_parameters){
    char number_of_parameters_string[MAX_DIGITS_OF_SCOPE];

    if(sprintf(number_of_parameters_string, "%d", number_of_parameters)<0)
        return INTERNAL_COMPILER_ERR;

    if( strAddConstStr(&output,"PUSHS int@")==STR_ERROR ||
        strAddConstStr(&output,number_of_parameters_string)==STR_ERROR ||
        strAddConstStr(&output,"\nCALL ")==STR_ERROR ||
        strAddConstStr(&output,name_of_function)==STR_ERROR ||
        strAddConstStr(&output,"\n")==STR_ERROR){
        return INTERNAL_COMPILER_ERR;
    }
    return OK;
}

// -------------------------------------------------------------------------------------------