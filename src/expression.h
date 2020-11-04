#pragma once
#include "scanner.h"
#include "symtable.h"

void parse_expression(table sym_table, token *token_arr, int token_count);
