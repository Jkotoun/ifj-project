
extern "C"
{
   #include "../src/str.h"
   #include "../src/scanner.h"
   #include "../src/error_codes.h"
   #include "../src/symtable.h"
   #include "../src/dl_list.h"
   #include "../src/expression.h"
}
#include <gtest/gtest.h>

class ExpressionParserTests : public ::testing::Test
{
   protected:
      void SetUp()
      {
      }
      void TearDown()
      {
      }
};

TEST_F(ExpressionParserTests, BasicTests)
{  
   tDLList scoped_symtables;
   DLInitList(&scoped_symtables);
   DLInsertLast(&scoped_symtables);
   symtable_init(&scoped_symtables.Last->root_ptr);
   token tokens[0];
   varType out_type;
   int expr_parser_output = parse_expression(&scoped_symtables,
      tokens, 0, &out_type);
   EXPECT_EQ(expr_parser_output, SYNTAX_ERR);
   EXPECT_EQ(out_type, UNDEFINED);
};

TEST_F(ExpressionParserTests, SameScopeVarAdditions)
{
   // SYMTABLE INIT
   // one symtable, variables a, b
   
   tDLList scoped_symtables;
   DLInitList(&scoped_symtables);
   DLInsertLast(&scoped_symtables);
   symtable_init(&scoped_symtables.Last->root_ptr);
   
   string param_a;
   strInit(&param_a);
   strAddChar(&param_a, 'a');      
   symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_a, INT);

   string param_b;
   strInit(&param_b);
   strAddChar(&param_b, 'b');      
   symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_b, INT);

   //INPUT TOKENS - EXPR
   // a + b
   token tokens[3] = {
      {
         .type = ID_TOKEN,
         .str = &param_a
      },
      {
         .type = PLUS_TOKEN
      },
      {
         .type = ID_TOKEN,
         .str = &param_a
      }
   };

   varType out_type;
   int expr_parser_output = parse_expression(&scoped_symtables,
      tokens, 3, &out_type);

   EXPECT_EQ(expr_parser_output, OK);
   EXPECT_EQ(out_type, INT);
};