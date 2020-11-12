
extern "C"
{
#include "../src/parser.h"
#include "../src/error_codes.h"
#include <dirent.h>
#include <stdio.h>
}
#include <gtest/gtest.h>

typedef std::pair<std::string, int> TestParam;

class ParserTests : public ::testing::TestWithParam<TestParam>
{
};

std::vector<TestParam> ReadTestCasesFromDisk(std::string testFolder)
{
    std::vector<TestParam> params;
    testFolder = "../../tests/parserTestSources/" + testFolder;
    DIR *d;
    struct dirent *dir;
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
                    params.push_back(TestParam(testFolder + "/" + dir->d_name, SYNTAX_ERR));
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
    testing::ValuesIn(ReadTestCasesFromDisk("basic")));

INSTANTIATE_TEST_CASE_P(
    Expressions,
    ParserTests,
    testing::ValuesIn(ReadTestCasesFromDisk("expressions")));

INSTANTIATE_TEST_CASE_P(
    Constructs,
    ParserTests,
    testing::ValuesIn(ReadTestCasesFromDisk("constructs")));

INSTANTIATE_TEST_CASE_P(
    FuncCalls,
    ParserTests,
    testing::ValuesIn(ReadTestCasesFromDisk("func_calls")));

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