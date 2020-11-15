#include "parser.h"
#include <stdio.h>

int main(int argc, char const* argv[])
{
    //TESTING ONLY
    stdin = fopen("../tests/parserTestSources/semantic/var_def_err/redef_var_scoped.go", "r");
    parser_start();
    // fprintf(stderr, "SUCC\n");
    return 0;
}
