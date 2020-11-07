#include "headers/parser.h"
#include <stdio.h>

int main(int argc, char const* argv[])
{
    stdin = fopen("../tests/parserTestSources/basic functions.go", "r");
    parser_start();
    fprintf(stderr, "SUCC\n");
    return 0;
}
