
extern "C"
{
#include "../src/headers/parser.h"
#include "../src/headers/error_codes.h"

}
#include <gtest/gtest.h>

typedef std::pair<std::string, int> TestParam;

class ParserTests : public ::testing::TestWithParam<TestParam> {};


TEST_P(ParserTests, Basic)
{
    std::string path = GetParam().first;
    int n = path.length();
    // declaring character array
    char char_array[n + 1];

    // copying the contents of the
    // string to char array
    strcpy(char_array, path.c_str());

    stdin = fopen(char_array, "r");
    ASSERT_EXIT(parser_start(), testing::ExitedWithCode(GetParam().second), "");
}


INSTANTIATE_TEST_CASE_P(
    GeneralAndSpecial,
    ParserTests,
    testing::Values(
        TestParam("../../tests/parserTestSources/basic/basic1.go", OK),
        TestParam("../../tests/parserTestSources/basic/basic2.go", OK),
        TestParam("../../tests/parserTestSources/basic/basic3.go", OK)
    ));

// TEST_F(ParserTests, BasicFunctions)
// {
//     stdin = fopen("../../tests/parserTestSources/basic functions.go", "r");
//     ASSERT_EXIT(parser_start(), testing::ExitedWithCode(OK), "");
// }

// TEST_F(ParserTests, BasicTest1)
// {
//     stdin = fopen("../../tests/parserTestSources/basic1.go", "r");
//     ASSERT_EXIT(parser_start(), testing::ExitedWithCode(OK), "");
// }

// TEST_F(ParserTests, BasicTest2)
// {
//     stdin = fopen("../../tests/parserTestSources/basic2.go", "r");
//     ASSERT_EXIT(parser_start(), testing::ExitedWithCode(OK), "");
// }

// TEST_F(ParserTests, BasicTest3)
// {
//     stdin = fopen("../../tests/parserTestSources/basic3.go", "r");
//     ASSERT_EXIT(parser_start(), testing::ExitedWithCode(OK), "");
// }