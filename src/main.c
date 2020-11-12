#include "headers/parser.h"
#include <stdio.h>

int main(int argc, char const* argv[])
{
    //TESTING ONLY
    stdin = fopen("../tests/parserTestSources/semantic/others/func_multiple_return_vars.go", "r");
    parser_start();
    fprintf(stderr, "SUCC\n");
    return 0;
}
