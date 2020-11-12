#include "scanner.h"

void func_handle_error(int errType, char const* func);

void get_next_token();

bool token_is(token_type token);

void func_assert_token_is(token_type token, char const* func);

bool keyword_is(keyword keyword);

void func_assert_keyword_is(keyword keyword, char const* func);

void func_assert_true(bool expression, int errno, char const* func);

bool types_equal(varType* types1, varType* types2, int length);

char* copystr(string* str);
