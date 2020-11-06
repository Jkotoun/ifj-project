#include "parser.h"
#include "stdio.h"

int main(int argc, char const* argv[])
{
    stdin = fopen("../test.go", "r");
    parser_start();
    fprintf(stderr, "SUCC\n");
    return 0;
}
