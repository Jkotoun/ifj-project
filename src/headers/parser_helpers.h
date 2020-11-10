#include "scanner.h"

void func_handle_error(int errType, char const *func);

void get_next_token();

bool token_is(token_type token);

void func_assert_token_is(token_type token, char const *func);

bool keyword_is(keyword keyword);

void func_assert_keyword_is(keyword keyword, char const *func);
