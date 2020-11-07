
extern "C"
{
#include "../src/headers/parser.h"
#include "../src/headers/error_codes.h"

}
#include <gtest/gtest.h>

class ParserTests : public ::testing::Test
{
protected:
    void SetUp()
    {
        // DLInitList(&list);
    }
    void TearDown()
    {
        // DLDisposeList(&list);
    }
};

TEST_F(ParserTests, BasicFunctions)
{
    stdin = fopen("../../tests/parserTestSources/basic functions.go", "r");
    ASSERT_EXIT(parser_start(), testing::ExitedWithCode(OK), "");
}

TEST_F(ParserTests, BasicTest1)
{
    stdin = fopen("../../tests/parserTestSources/basic1.go", "r");
    ASSERT_EXIT(parser_start(), testing::ExitedWithCode(OK), "");
}

TEST_F(ParserTests, BasicTest2)
{
    stdin = fopen("../../tests/parserTestSources/basic2.go", "r");
    ASSERT_EXIT(parser_start(), testing::ExitedWithCode(OK), "");
}

TEST_F(ParserTests, BasicTest3)
{
    stdin = fopen("../../tests/parserTestSources/basic3.go", "r");
    ASSERT_EXIT(parser_start(), testing::ExitedWithCode(OK), "");
}