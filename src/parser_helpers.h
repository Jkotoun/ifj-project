#include "scanner.h"

void func_handle_error(int errType, char const* func);

void get_next_token();

bool token_is(token_type token);

void func_assert_token_is(token_type token, char const* func);

bool keyword_is(keyword keyword);

void func_assert_keyword_is(keyword keyword, char const* func);

void func_assert_true(bool expression, int errno, char const* func);

bool types_equal(varType* types1, varType* types2, int length);

bool check_var_defined(string* varName);

void def_var(token* varToken, varType type);

void def_func(string* func_name, varType* paramArr, int paramArrLength, varType* returnArr, int returnArrLength, bool definition);

char* copystr(string* str);

int get_varType_from_symtable(string* varName, varType* type);

varType get_varType_from_literal(token_type type);

string* get_token_str(token* token);

varType* tokenArr_to_varTypeArr(token* tokenArr, int count);
