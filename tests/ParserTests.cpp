
extern "C"
{
#include "../src/headers/parser.h"
#include "../src/headers/error_codes.h"
#include <dirent.h>
#include <stdio.h>
}
#include <gtest/gtest.h>

typedef std::pair<std::string, int> TestParam;

class ParserTests : public ::testing::TestWithParam<TestParam>
{
};

std::vector<TestParam> ReadTestCasesFromDisk(std::string testFolder, int errcode)
{
    std::vector<TestParam> params;
    testFolder = "../../tests/parserTestSources/" + testFolder;
    DIR* d;
    struct dirent* dir;
    int n = testFolder.length();
    // declaring character array
    char char_array[n + 1];

    // copying the contents of the
    // string to char array
    strcpy(char_array, testFolder.c_str());
    d = opendir(char_array);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (dir->d_type == DT_REG)
            {
                if (std::string(dir->d_name).find("-e") == -1)
                {
                    params.push_back(TestParam(testFolder + "/" + dir->d_name, OK));
                }
                else
                {
                    params.push_back(TestParam(testFolder + "/" + dir->d_name, errcode));
                }
            }
        }
        closedir(d);
    }
    return params;
}

TEST_P(ParserTests, ExitCodeMatches)
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
    Basic,
    ParserTests,
    testing::ValuesIn(ReadTestCasesFromDisk("basic", SYNTAX_ERR)));

INSTANTIATE_TEST_CASE_P(
    Expressions,
    ParserTests,
    testing::ValuesIn(ReadTestCasesFromDisk("expressions", SYNTAX_ERR)));

INSTANTIATE_TEST_CASE_P(
    Constructs,
    ParserTests,
    testing::ValuesIn(ReadTestCasesFromDisk("constructs", SYNTAX_ERR)));

INSTANTIATE_TEST_CASE_P(
    FuncCalls,
    ParserTests,
    testing::ValuesIn(ReadTestCasesFromDisk("func_calls", SYNTAX_ERR)));

INSTANTIATE_TEST_CASE_P(
    ARGS_RETURNS_COUNT,
    ParserTests,
    testing::ValuesIn(ReadTestCasesFromDisk("semantic/args_returns_count", ARGS_RETURNS_COUNT_ERR)));

INSTANTIATE_TEST_CASE_P(
    DivideByZero,
    ParserTests,
    testing::ValuesIn(ReadTestCasesFromDisk("semantic/divide_zero", DIVIDE_ZERO_ERR)));

INSTANTIATE_TEST_CASE_P(
    ExpressionCompatibility,
    ParserTests,
    testing::ValuesIn(ReadTestCasesFromDisk("semantic/expr_compat", DATATYPE_COMPATIBILITY_ERR)));

INSTANTIATE_TEST_CASE_P(
    OtherSemantics,
    ParserTests,
    testing::ValuesIn(ReadTestCasesFromDisk("semantic/others", OTHER_SEMANTIC_ERR)));

INSTANTIATE_TEST_CASE_P(
    Definitions,
    ParserTests,
    testing::ValuesIn(ReadTestCasesFromDisk("semantic/var_def_err", VAR_DEFINITION_ERR))
);

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