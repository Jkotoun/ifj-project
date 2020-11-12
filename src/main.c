#include "headers/parser.h"
#include <stdio.h>

int main(int argc, char const* argv[])
{
    //TESTING ONLY
    stdin = fopen("../tests/parserTestSources/semantic/args_returns_count/func_wrong_return_number-e.go", "r");
    parser_start();
    fprintf(stderr, "SUCC\n");
    return 0;
}
