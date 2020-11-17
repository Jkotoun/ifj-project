cmake_minimum_required(VERSION 3.8)

add_executable(ScannerTest ScannerTests.cpp)
target_link_libraries(ScannerTest PUBLIC
    gtest_main
    IFJ_compiler
)
add_test(NAME ScannerTest COMMAND ScannerTest)

add_executable(BTreeTest BTreeTests.cpp)
target_link_libraries(BTreeTest PUBLIC
    gtest_main
    IFJ_compiler
)
add_test(NAME BTreeTest COMMAND BTreeTest)

add_executable(StrLibTest StrLibTests.cpp)
target_link_libraries(StrLibTest PUBLIC
    gtest_main
    IFJ_compiler
)
add_test(NAME StrLibTest COMMAND StrLibTest)

add_executable(ListTest ListTests.cpp)
target_link_libraries(ListTest PUBLIC
    gtest_main
    IFJ_compiler
)
add_test(NAME ListTest COMMAND ListTest)

add_executable(ParserTest ParserTests.cpp)
target_link_libraries(ParserTest PUBLIC
    gtest_main
    IFJ_compiler
)
add_test(NAME ParserTest COMMAND ParserTest)

add_executable(ExpressionParserTests ExpressionParserTests.cpp)
target_link_libraries(ExpressionParserTests PUBLIC
    gtest_main
    IFJ_compiler
)
add_test(NAME ExpressionParserTests COMMAND ExpressionParserTests)