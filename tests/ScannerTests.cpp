
extern "C"
{
#include "../src/headers/str.h"
#include "../src/headers/scanner.h"
#include "../src/headers/error_codes.h"
}
#include <gtest/gtest.h>

class ScannerTests : public ::testing::Test
{
protected:
   string str;
   token token_var;
   void SetUp()
   {

      strInit(&str);
      token_var.str = &str;
   }
   void TearDown()
   {
      strFree(&str);
   }
};
TEST_F(ScannerTests, BasicLexemsTest)
{
   stdin = fopen("../../tests/basicLexemsSource", "r");
   token_type tokens[] = {MULTIPLICATION_TOKEN, GREATER_TOKEN, GREATER_EQUAL_TOKEN,
                          LESS_TOKEN, LESS_EQUAL_TOKEN, NOT_EQUALS_TOKEN, ASSIGNMENT_TOKEN, EQUALS_TOKEN,
                          MINUS_TOKEN, SEMICOLON_TOKEN, COMMA_TOKEN, PLUS_TOKEN, SHORT_VAR_DECLARATION_TOKEN,
                          RIGHT_BRACKET_TOKEN, LEFT_BRACKET_TOKEN, CURLY_BRACKET_LEFT_TOKEN, CURLY_BRACKET_RIGHT_TOKEN,
                          DIVISON_TOKEN, EOF_TOKEN};
   int i = 0;
   do
   {
      get_token(&token_var);
      EXPECT_EQ(token_var.type, tokens[i]);
      i++;
   } while (token_var.type != EOF_TOKEN);
   fclose(stdin);
}

TEST_F(ScannerTests, CommentsRemoveTest)
{
   stdin = fopen("../../tests/commentsRemoveSource", "r");
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, INTEGER_LITERAL_TOKEN);
   EXPECT_EQ(token_var.integer, 123);
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, KEYWORD_TOKEN);
   EXPECT_EQ(token_var.keyword, IF_KEYWORD);
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, KEYWORD_TOKEN);
   EXPECT_EQ(token_var.keyword, ELSE_KEYWORD);
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, EOL_TOKEN);
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, KEYWORD_TOKEN);
   EXPECT_EQ(token_var.keyword, FOR_KEYWORD);
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, EOL_TOKEN);
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, INTEGER_LITERAL_TOKEN);
   EXPECT_EQ(token_var.integer, 1234);
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, EOF_TOKEN);
   fclose(stdin);
}

TEST_F(ScannerTests, NumbersLiteralsTest)
{
   stdin = fopen("../../tests/numbersTestSource", "r");
   //first num
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(INTEGER_LITERAL_TOKEN, token_var.type);
   EXPECT_EQ(1284342347, token_var.integer);
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(EOL_TOKEN, token_var.type);
   //second num
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, DECIMAL_LITERAL_TOKEN);
   EXPECT_EQ(token_var.decimal, 123.45);
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, EOL_TOKEN);
   //3. number
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, DECIMAL_LITERAL_TOKEN);
   EXPECT_EQ(token_var.decimal, 123e+2);
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, EOL_TOKEN);
   //4. number
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, DECIMAL_LITERAL_TOKEN);
   EXPECT_DOUBLE_EQ(token_var.decimal, 123.43e-1);
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, EOL_TOKEN);
   //5. number
   EXPECT_EQ(get_token(&token_var), LEX_ERR);
   //6. number
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_DOUBLE_EQ(token_var.decimal, 0.2313);
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, EOL_TOKEN);
   //7.num
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_DOUBLE_EQ(token_var.decimal, 123.0004);
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, EOL_TOKEN);
   //8. number
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.integer, 0);
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, EOL_TOKEN);
   //9. number
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_DOUBLE_EQ(token_var.decimal, 0.0);
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, EOL_TOKEN);

   //last num with 0 at start
   EXPECT_EQ(get_token(&token_var), LEX_ERR);
   fclose(stdin);
}

TEST_F(ScannerTests, StringTest)
{
   stdin = fopen("../../tests/stringTestSource", "r");

   EXPECT_EQ(get_token(&token_var), OK);

   EXPECT_EQ(token_var.type, STRING_LITERAL_TOKEN);
   char string_expected[] = {"test hexaK newline\n tab\t quote\" slash\\\0"};
   int i = 0;
   char *string_got = strGetStr(token_var.str);
   
   while(string_expected[i]!='\0')
   {
      EXPECT_EQ(string_expected[i], string_got[i]);
      i++;
   }
   fclose(stdin);
}

TEST_F(ScannerTests, KeywordsAndIdTest)
{
   stdin = fopen("../../tests/keywordsIdsSource", "r");
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(KEYWORD_TOKEN, token_var.type);
   EXPECT_EQ(FLOAT64_KEYWORD, token_var.keyword);
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(ID_TOKEN, token_var.type);
   char idcko[] = "_varname1";
   EXPECT_TRUE(strCmpConstStr(token_var.str, idcko) == 0);
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(EOL_TOKEN, token_var.type);
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, KEYWORD_TOKEN);
   EXPECT_EQ(token_var.keyword, STRING_KEYWORD);
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, ID_TOKEN);
   char idcko2[] = "varname2";
   EXPECT_TRUE(strCmpConstStr(token_var.str, idcko2) == 0);
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(EOL_TOKEN, token_var.type);
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, KEYWORD_TOKEN);
   EXPECT_EQ(token_var.keyword, INT_KEYWORD);
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, ID_TOKEN);
   char idcko3[] = "Number";
   EXPECT_TRUE(strCmpConstStr(token_var.str, idcko3) == 0);
   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, ASSIGNMENT_TOKEN);

   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, INTEGER_LITERAL_TOKEN);
   EXPECT_EQ(token_var.integer, 23);

   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, EOL_TOKEN);

   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, KEYWORD_TOKEN);
   EXPECT_EQ(token_var.keyword, IF_KEYWORD);

   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, KEYWORD_TOKEN);
   EXPECT_EQ(token_var.keyword, ELSE_KEYWORD);

   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, KEYWORD_TOKEN);
   EXPECT_EQ(token_var.keyword, FOR_KEYWORD);

   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, KEYWORD_TOKEN);
   EXPECT_EQ(token_var.keyword, FUNC_KEYWORD);

   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, KEYWORD_TOKEN);
   EXPECT_EQ(token_var.keyword, PACKAGE_KEYWORD);

   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, KEYWORD_TOKEN);
   EXPECT_EQ(token_var.keyword, RETURN_KEYWORD);

   EXPECT_EQ(get_token(&token_var), OK);
   EXPECT_EQ(token_var.type, EOF_TOKEN);
   fclose(stdin);
}