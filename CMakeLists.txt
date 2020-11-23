cmake_minimum_required(VERSION 3.8)

set (Project_name IFJ_compiler)
project (${Project_name} C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

enable_testing()

add_subdirectory(tests/googletest)

set(Headers
    src/error_codes.h;
    src/scanner.h;
    src/str.h;
    src/dl_list.h;
    src/expression_stack.h;
    src/expression.h;
    src/symtable.h;
    src/parser.h;
    src/parser_rules.h;
    src/parser_helpers.h;
    src/queue.h;
    src/expression_stack.h;
    src/code_gen.h;
)
set(Sources
    src/str.c 
    src/scanner.c
    src/symtable.c
    src/dl_list.c
    src/parser.c
    src/expression_stack.c
    src/expression.c
    src/queue.c
    src/expression.c;
    src/expression_stack.c;
    src/code_gen.c;
)
add_library(${Project_name} STATIC ${Sources} ${Headers})
add_subdirectory(tests)