extern "C"
{
#include "../src/str.h"
#include "../src/scanner.h"
#include "../src/error_codes.h"
}
#include <gtest/gtest.h>

class CodeGenTest : public ::testing::Test
{
protected:
//deklarace
   string str;
   token token_var;
   string raw_str;
   void SetUp()
   {
//nastavení proměných
   }
   void TearDown()
   {
    //uvolnění proměných  
    }
};


TEST_F(CodeGenTest,nz){
    printf("a jede to");
}