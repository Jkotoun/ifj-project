#include "scanner.h"

void handle_error(int errType);

void get_next_token();


bool token_is(token_type token);

void assert_token_is(token_type token);

bool keyword_is(keyword keyword);

void assert_keyword_is(keyword keyword);
