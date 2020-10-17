#ifndef err_codes
#define err_codes

//lexical analysis 
#define OK 0
#define LEX_ERR 1

//syntax analysis errors
#define SYNTAX_ERR 2

//semantic analysis errors
#define VAR_DEFINITION_ERR 3 //undefined function/variable or redefinition


#define DEFINED_VAR_DATATYPE_ERR 4 //data type error of new defined variable 
#define DATATYPE_COMPATIBILITY_ERR 5 //data types incompatibility in expression

#define ARGS_RETURNS_COUNT_ERR 6 //wrong count or type of arguments or returns in function

#define OTHER_SEMANTIC_ERR 7
#define DIVIDE_ZERO_ERR 9

//internal compiler errors like malloc error etc.
#define INTERNAL_COMPILER_ERR 99 




#endif

