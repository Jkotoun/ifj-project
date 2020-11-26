
extern "C" {
#include "../src/str.h"
#include "../src/scanner.h"
#include "../src/error_codes.h"
}
#include <gtest/gtest.h>

class StrLibTests : public ::testing::Test
{
    protected:
    
    void SetUp() 
    {
        
   }
   void TearDown() 
   {
   }
};
TEST_F(StrLibTests, AddStringTest)
{
    string output;
    strInit(&output);
    strAddChar(&output, 'a');
    strAddChar(&output, 'b');


    char str_const[] = {'c', 'd', 'e', 'f', 'g', '\0'};
    EXPECT_EQ(strAddConstStr(&output, str_const), STR_SUCCESS);
    char str_expected[] = "abcdefg";
    EXPECT_EQ(strCmpConstStr(&output,str_expected),0);


    strClear(&output);
    char str[] = "bozena";
    EXPECT_EQ(strAddConstStr(&output, str), STR_SUCCESS);
    EXPECT_EQ(strCmpConstStr(&output, str), 0);

    strFree(&output);

}
