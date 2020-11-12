cmake_minimum_required(VERSION 3.8)

set (Project_name IFJ_compiler)
project (${Project_name} C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

enable_testing()

add_subdirectory(tests/googletest)

set(Headers
    src/headers/error_codes.h;
    src/headers/scanner.h;
    src/headers/str.h;
    src/headers/dl_list.h;
    src/headers/code_gen.h;
    src/headers/expression.h;
    src/headers/symtable.h;
    src/headers/parser.h;
    src/headers/parser_rules.h;
    src/headers/parser_helpers.h;
    src/headers/varType_queue.h;
)
set(Sources
    src/str.c 
    src/scanner.c
    src/symtable.c
    src/dl_list.c
    src/parser.c
    src/varType_queue.c
)
add_library(${Project_name} STATIC ${Sources} ${Headers})
add_subdirectory(tests)