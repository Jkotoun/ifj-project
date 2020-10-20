cmake_minimum_required(VERSION 3.8)



add_executable(ScannerTest ScannerTests.cpp)

target_link_libraries(ScannerTest PUBLIC
    gtest_main
    IFJ_compiler
)

add_test(NAME ScannerTest COMMAND ScannerTest)