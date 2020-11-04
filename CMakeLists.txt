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
)
set(Sources
    src/str.c 
    src/scanner.c
)
add_library(${Project_name} STATIC ${Sources} ${Headers})
add_subdirectory(tests)