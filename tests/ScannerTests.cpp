
extern "C" {
#include "../src/str.h"
#include "../src/scanner.h"
}
#include <gtest/gtest.h>


TEST(ScannerTests,BasicTest)
{
   string str;
    strInit(&str);
    strAddChar(&str,'i');
    strAddChar(&str,'f');
    keyword keyword;

    
   // EXPECT_TRUE(IF_KEYWORD == IF_KEYWORD);
    
    EXPECT_TRUE(str_is_keyword(&str,&keyword));
    EXPECT_EQ(keyword,IF_KEYWORD);
   // EXPECT_TRUE(kw_var==IF_KEYWORD);
    
}