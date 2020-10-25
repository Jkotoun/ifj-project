
extern "C" {
#include "../src/str.h"
#include "../src/scanner.h"
#include "../src/error_codes.h"
}
#include <gtest/gtest.h>

class StrLibTests : public ::testing::Test
{
    protected:
    string str1;
    string str2;
    void SetUp() 
    {
        strInit(&str1);
        char test1[] = {'a', 'b', 'c'};
        for(int i = 0;i<(int)sizeof(test1);i++)
        {
            ASSERT_NE(strAddChar(&str1, test1[i]),STR_ERROR);
        }
        strInit(&str2);
        char test2[] = {'d', 'e', 'f'};
        for(int i = 0;i<(int)sizeof(test2);i++)
        {
            ASSERT_NE(strAddChar(&str2, test2[i]),STR_ERROR);
        }
   }
   void TearDown() 
   {
      strFree(&str1);
      strFree(&str2);
   }
};
TEST_F(StrLibTests, ConcatenationTest)
{
    string concat_str;
    ASSERT_NE(strInit(&concat_str), STR_ERROR);
    ASSERT_EQ(strConcat(&str1, &str2, &concat_str), STR_ERROR);
    EXPECT_EQ(strGetLength(&str1), 3);
    EXPECT_EQ(strGetLength(&str2), 3);
    EXPECT_EQ(strGetLength(&concat_str), 6);
    char expected_out[] = {'a', 'b', 'c', 'd', 'e', 'f', '\0'};
    EXPECT_EQ(strCmpConstStr(&concat_str, expected_out), 0);
    strFree(&concat_str);
}
