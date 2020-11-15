#include "parser.h"
#include <stdio.h>

int main(int argc, char const* argv[])
{
    //TESTING ONLY
<<<<<<< HEAD
    // stdin = fopen("../tests/parserTestSources/expressions/expr2.go", "r");
=======
    stdin = fopen("../tests/parserTestSources/semantic/others/func_multiple_return_vars.go", "r");
>>>>>>> parser_semantic_anal
    parser_start();
    // fprintf(stderr, "SUCC\n");
    return 0;
}
