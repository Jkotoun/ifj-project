
extern "C" {
#include "../src/headers/str.h"
#include "../src/headers/scanner.h"
#include "../src/headers/error_codes.h"
}
#include <gtest/gtest.h>

class StrLibTests : public ::testing::Test
{
    protected:
    string str1;
    string str2;
    string str3;
    void init_str_chars(char chars[], int count,  string *str)
    {
        strInit(str);
        for(int i = 0;i<count;i++)
        {
            ASSERT_NE(strAddChar(str, chars[i]),STR_ERROR);
        }
    }
    void SetUp() 
    {
        char test1[] = {'a', 'b', 'c'};
        init_str_chars(test1, sizeof(test1),&str1);
        

        char test2[] = {'d', 'e', 'f'};
        init_str_chars(test2, sizeof(test2), &str2);

       
        char test3[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};
        init_str_chars(test3,sizeof(test3), &str3);
        
   }
   void TearDown() 
   {
      strFree(&str1);
      strFree(&str2);
      strFree(&str3);
   }
};
TEST_F(StrLibTests, ConcatenationTest)
{
    string concat_str;
    ASSERT_NE(strInit(&concat_str), STR_ERROR);
    ASSERT_NE(strConcat(&str1, &str2, &concat_str), STR_ERROR);
    EXPECT_EQ(strGetLength(&str1), 3);
    EXPECT_EQ(strGetLength(&str2), 3);
    EXPECT_EQ(strGetLength(&concat_str), 6);
    char expected_out[] = {'a', 'b', 'c', 'd', 'e', 'f', '\0'};
    EXPECT_EQ(strCmpConstStr(&concat_str, expected_out), 0);
    strFree(&concat_str);
}

TEST_F(StrLibTests, SubstringTest)
{
    string output;
    ASSERT_EQ(strInit(&output), STR_SUCCESS);
    strClear(&output);
    ASSERT_EQ(substr(&str3,0,5,&output), STR_SUCCESS);
    char expected_out[] = "abcde";
    EXPECT_EQ(strCmpConstStr(&output, expected_out), 0);
    strClear(&output);
    
    ASSERT_EQ(substr(&str3, 4,2,&output), STR_SUCCESS);
    char expected_out2[] = "ef";
    EXPECT_EQ(strCmpConstStr(&output,expected_out2), 0);

    strClear(&output);
    char expected_out3[] = "hij";
    ASSERT_EQ(substr(&str3, 7, 5, &output), STR_SUCCESS);
    EXPECT_EQ(strCmpConstStr(&output, expected_out3),0);

    EXPECT_EQ(substr(&str3, 52,2,&output), STR_ERROR);
    EXPECT_EQ(substr(&str3, 5,-2,&output), STR_ERROR);
    
    EXPECT_EQ(substr(&str3, -5,2,&output), STR_ERROR);
    strFree(&output);
}

TEST_F(StrLibTests, StrOrdTest)
{
    int output;
    EXPECT_EQ(str_ord(&str3,2,&output),STR_SUCCESS);
    EXPECT_EQ(output, (int)'c');
    EXPECT_EQ(str_ord(&str3,50,&output),STR_ERROR);
    EXPECT_EQ(str_ord(&str3,-2,&output),STR_ERROR);
}
TEST_F(StrLibTests, AsciiToStrTest)
{

    string output;
    ASSERT_EQ(strInit(&output), STR_SUCCESS);
    EXPECT_EQ(ascii_to_string(75, &output), STR_SUCCESS);
    char expected_out[] = "K";
    EXPECT_EQ(strCmpConstStr(&output,expected_out), 0);

    //test of passing non-empty string
    strAddChar(&output, 'F');
    strAddChar(&output, 'g');
    char expected_out2[] = "P";
    EXPECT_EQ(ascii_to_string(80, &output), STR_SUCCESS);
    EXPECT_EQ(strCmpConstStr(&output,expected_out2), 0);

    EXPECT_EQ(ascii_to_string(270, &output), STR_ERROR);
    EXPECT_EQ(ascii_to_string(-2, &output), STR_ERROR);

    strFree(&output);
}
