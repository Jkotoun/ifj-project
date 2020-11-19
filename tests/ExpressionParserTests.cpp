
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

TEST_F(ExpressionParserTests, EmptyTokensTests)
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

TEST_F(ExpressionParserTests, UnexpectedTokensTests)
{    
   tDLList scoped_symtables;
   DLInitList(&scoped_symtables);
   
   for(token_type type : {KEYWORD_TOKEN, EOL_TOKEN, EOF_TOKEN, SHORT_VAR_DECLARATION_TOKEN, 
      CURLY_BRACKET_LEFT_TOKEN, CURLY_BRACKET_RIGHT_TOKEN, COMMA_TOKEN, 
      ASSIGNMENT_TOKEN, SEMICOLON_TOKEN} )
   {
      token tokens[3] = {
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 50
         },
         {
            .type = PLUS_TOKEN
         },
         {
            .type = type
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, SYNTAX_ERR);
      EXPECT_EQ(out_type, UNDEFINED);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, INT);
   }      
};
 
TEST_F(ExpressionParserTests, LiteralsTests)
{   
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
     
      token tokens[3] = {
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 50
         },
         {
            .type = PLUS_TOKEN
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 5
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, INT);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, INT);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, INT);
      EXPECT_EQ(reduction_steps->next->next->rule, nt_plus_nt);
      EXPECT_EQ(reduction_steps->next->next->type, INT);
   }

   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);

      token tokens[3] = {
         {
            .type = DECIMAL_LITERAL_TOKEN,
            .decimal = 556.89
         },
         {
            .type = PLUS_TOKEN
         },
         {
            .type = DECIMAL_LITERAL_TOKEN,
            .decimal = 5.56
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, FLOAT);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, FLOAT);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, FLOAT);
      EXPECT_EQ(reduction_steps->next->next->rule, nt_plus_nt);
      EXPECT_EQ(reduction_steps->next->next->type, FLOAT);
   }
};

TEST_F(ExpressionParserTests, VariablesTests)
{   
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
      DLInsertLast(&scoped_symtables);
      symtable_init(&scoped_symtables.Last->root_ptr);

      string param_c;
      strInit(&param_c);
      strAddChar(&param_c, 'c');      
      symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_c, INT);
      
      string param_d;
      strInit(&param_d);
      strAddChar(&param_d, 'd');      
      symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_d, INT);

      token tokens[3] = {
         {
            .type = ID_TOKEN,
            .str = &param_c
         },
         {
            .type = PLUS_TOKEN
         },
         {
            .type = ID_TOKEN,
            .str = &param_d
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, INT);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, INT);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, INT);
      EXPECT_EQ(reduction_steps->next->next->rule, nt_plus_nt);
      EXPECT_EQ(reduction_steps->next->next->type, INT);
   }

   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
      DLInsertLast(&scoped_symtables);
      symtable_init(&scoped_symtables.Last->root_ptr);

      string param_c;
      strInit(&param_c);
      strAddChar(&param_c, 'c');      
      symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_c, FLOAT);
      
      string param_d;
      strInit(&param_d);
      strAddChar(&param_d, 'd');      
      symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_d, FLOAT);

      token tokens[3] = {
         {
            .type = ID_TOKEN,
            .str = &param_c
         },
         {
            .type = PLUS_TOKEN
         },
         {
            .type = ID_TOKEN,
            .str = &param_d
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, FLOAT);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, FLOAT);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, FLOAT);
      EXPECT_EQ(reduction_steps->next->next->rule, nt_plus_nt);
      EXPECT_EQ(reduction_steps->next->next->type, FLOAT);
   }
};

TEST_F(ExpressionParserTests, VariablesScopesTests)
{
   tDLList scoped_symtables;
   DLInitList(&scoped_symtables);
   DLInsertLast(&scoped_symtables);
   symtable_init(&scoped_symtables.Last->root_ptr);
   
   // Scope 0
   string param_a;
   strInit(&param_a);
   strAddChar(&param_a, 'a');      
   symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_a, INT);

   string param_b;
   strInit(&param_b);
   strAddChar(&param_b, 'b');      
   symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_b, INT);

   // Scope 1
   DLInsertLast(&scoped_symtables);
   symtable_init(&scoped_symtables.Last->root_ptr);
   string param_c;
   strInit(&param_c);
   strAddChar(&param_c, 'c');      
   symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_c, INT);

   //INPUT TOKENS - EXPR
   // a + b + c
   token tokens[5] = {
      {
         .type = ID_TOKEN,
         .str = &param_a
      },
      {
         .type = PLUS_TOKEN
      },
      {
         .type = ID_TOKEN,
         .str = &param_b
      },
      {
         .type = PLUS_TOKEN
      },
      {
         .type = ID_TOKEN,
         .str = &param_c
      }
   };

   varType out_type;
   instrumented_node *reduction_steps = NULL;
   int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
      tokens, 3, &out_type, &reduction_steps);

   EXPECT_EQ(expr_parser_output, OK);
   EXPECT_EQ(out_type, INT);
   EXPECT_EQ(reduction_steps->rule, operand);
   EXPECT_EQ(reduction_steps->type, INT);
   EXPECT_EQ(reduction_steps->next->rule, operand);
   EXPECT_EQ(reduction_steps->next->type, INT);
   EXPECT_EQ(reduction_steps->next->next->rule, nt_plus_nt);
   EXPECT_EQ(reduction_steps->next->next->type, INT);
   EXPECT_EQ(reduction_steps->rule, operand);
   EXPECT_EQ(reduction_steps->type, INT);
   EXPECT_EQ(reduction_steps->next->next->rule, nt_plus_nt);
   EXPECT_EQ(reduction_steps->next->next->type, INT);
};

TEST_F(ExpressionParserTests, LiteralsVariablesCombinationTests)
{    
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
      DLInsertLast(&scoped_symtables);
      symtable_init(&scoped_symtables.Last->root_ptr);

      string param_c;
      strInit(&param_c);
      strAddChar(&param_c, 'c');      
      symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_c, INT);
      
      token tokens[3] = {
         {
            .type = ID_TOKEN,
            .str = &param_c
         },
         {
            .type = PLUS_TOKEN
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 5
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, INT);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, INT);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, INT);
      EXPECT_EQ(reduction_steps->next->next->rule, nt_plus_nt);
      EXPECT_EQ(reduction_steps->next->next->type, INT);
   }

   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
      DLInsertLast(&scoped_symtables);
      symtable_init(&scoped_symtables.Last->root_ptr);

      string param_c;
      strInit(&param_c);
      strAddChar(&param_c, 'c');      
      symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_c, FLOAT);
          
      token tokens[3] = {
         {
            .type = ID_TOKEN,
            .str = &param_c
         },
         {
            .type = PLUS_TOKEN
         },
         {
            .type = DECIMAL_LITERAL_TOKEN,
            .decimal = 5.56
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, FLOAT);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, FLOAT);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, FLOAT);
      EXPECT_EQ(reduction_steps->next->next->rule, nt_plus_nt);
      EXPECT_EQ(reduction_steps->next->next->type, FLOAT);
   }
};

TEST_F(ExpressionParserTests, StrConcatTest)
{  
   tDLList scoped_symtables;
   DLInitList(&scoped_symtables);
   DLInsertLast(&scoped_symtables);
   symtable_init(&scoped_symtables.Last->root_ptr);

   string str_A;
   strInit(&str_A);
   strAddChar(&str_A, 'A');      

   string str_B;
   strInit(&str_B);
   strAddChar(&str_B, 'B');      

   // Literals
   {
      token tokens[3] = {
         {
            .type = STRING_LITERAL_TOKEN,
            .str = &str_A
         },
         {
            .type = PLUS_TOKEN
         },
         {
            .type = STRING_LITERAL_TOKEN,
            .str = &str_B
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, STRING);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, STRING);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, STRING);
      EXPECT_EQ(reduction_steps->next->next->rule, nt_plus_nt);
      EXPECT_EQ(reduction_steps->next->next->type, STRING);
   }

   // Variables
   {
      string param_c;
      strInit(&param_c);
      strAddChar(&param_c, 'c');      
      symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_c, STRING);
      
      string param_d;
      strInit(&param_d);
      strAddChar(&param_d, 'd');      
      symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_d, STRING);

      token tokens[3] = {
         {
            .type = ID_TOKEN,
            .str = &param_c
         },
         {
            .type = PLUS_TOKEN
         },
         {
            .type = ID_TOKEN,
            .str = &param_d
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, STRING);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, STRING);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, STRING);
      EXPECT_EQ(reduction_steps->next->next->rule, nt_plus_nt);
      EXPECT_EQ(reduction_steps->next->next->type, STRING);
   }

   // Literals + variable
   {
      string param_c;
      strInit(&param_c);
      strAddChar(&param_c, 'c');      
      symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_c, STRING);

      token tokens[5] = {
         {
            .type = STRING_LITERAL_TOKEN,
            .str = &str_A
         },
         {
            .type = PLUS_TOKEN
         },
         {
            .type = STRING_LITERAL_TOKEN,
            .str = &str_B
         },
         {
            .type = PLUS_TOKEN
         },
         {
            .type = ID_TOKEN,
            .str = &param_c
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 5, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, STRING);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, STRING);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, STRING);
      EXPECT_EQ(reduction_steps->next->next->rule, nt_plus_nt);
      EXPECT_EQ(reduction_steps->next->next->type, STRING);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, STRING);
      EXPECT_EQ(reduction_steps->next->next->rule, nt_plus_nt);
      EXPECT_EQ(reduction_steps->next->next->type, STRING);
   }
};

TEST_F(ExpressionParserTests, StrRelationalCmpTests)
{  
   tDLList scoped_symtables;
   DLInitList(&scoped_symtables);
   DLInsertLast(&scoped_symtables);
   symtable_init(&scoped_symtables.Last->root_ptr);

   string str_A;
   strInit(&str_A);
   strAddChar(&str_A, 'A');      

   string param_b;
   strInit(&param_b);
   strAddChar(&param_b, 'b');      
   symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_b, STRING);

   {
      token tokens[3] = {
         {
            .type = STRING_LITERAL_TOKEN,
            .str = &str_A
         },
         {
            .type = EQUALS_TOKEN
         },
         {
            .type = ID_TOKEN,
            .str = &param_b
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, BOOL);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, STRING);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, STRING);
      EXPECT_EQ(reduction_steps->next->next->rule, nt_eq_nt);
      EXPECT_EQ(reduction_steps->next->next->type, BOOL);
   }

   {
      token tokens[3] = {
         {
            .type = STRING_LITERAL_TOKEN,
            .str = &str_A
         },
         {
            .type = NOT_EQUALS_TOKEN
         },
         {
            .type = ID_TOKEN,
            .str = &param_b
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, BOOL);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, STRING);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, STRING);
      EXPECT_EQ(reduction_steps->next->next->rule, nt_neq_nt);
      EXPECT_EQ(reduction_steps->next->next->type, BOOL);
   }

   {
      token tokens[3] = {
         {
            .type = STRING_LITERAL_TOKEN,
            .str = &str_A
         },
         {
            .type = LESS_EQUAL_TOKEN
         },
         {
            .type = ID_TOKEN,
            .str = &param_b
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, BOOL);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, STRING);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, STRING);
      EXPECT_EQ(reduction_steps->next->next->rule, nt_less_eq_nt);
      EXPECT_EQ(reduction_steps->next->next->type, BOOL);
   }

   {
      token tokens[3] = {
         {
            .type = STRING_LITERAL_TOKEN,
            .str = &str_A
         },
         {
            .type = GREATER_EQUAL_TOKEN
         },
         {
            .type = ID_TOKEN,
            .str = &param_b
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, BOOL);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, STRING);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, STRING);
      EXPECT_EQ(reduction_steps->next->next->rule, nt_more_eq_nt);
      EXPECT_EQ(reduction_steps->next->next->type, BOOL);
   }

   {
      token tokens[3] = {
         {
            .type = STRING_LITERAL_TOKEN,
            .str = &str_A
         },
         {
            .type = LESS_TOKEN
         },
         {
            .type = ID_TOKEN,
            .str = &param_b
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, BOOL);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, STRING);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, STRING);
      EXPECT_EQ(reduction_steps->next->next->rule, nt_less_nt);
      EXPECT_EQ(reduction_steps->next->next->type, BOOL);
   }

   {
      token tokens[3] = {
         {
            .type = STRING_LITERAL_TOKEN,
            .str = &str_A
         },
         {
            .type = GREATER_TOKEN
         },
         {
            .type = ID_TOKEN,
            .str = &param_b
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, BOOL);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, STRING);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, STRING);
      EXPECT_EQ(reduction_steps->next->next->rule, nt_more_nt);
      EXPECT_EQ(reduction_steps->next->next->type, BOOL);
   }
};

TEST_F(ExpressionParserTests, StrInvalidOperationTests)
{
   string str_A;
   strInit(&str_A);
   strAddChar(&str_A, 'A');      
   
   tDLList scoped_symtables;
   DLInitList(&scoped_symtables);
   DLInsertLast(&scoped_symtables);
   symtable_init(&scoped_symtables.Last->root_ptr);

   string param_b;
   strInit(&param_b);
   strAddChar(&param_b, 'b');      
   symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_b, STRING);

   {
      token tokens[3] = {
         {
            .type = STRING_LITERAL_TOKEN,
            .str = &str_A
         },
         {
            .type = MULTIPLICATION_TOKEN
         },
         {
            .type = ID_TOKEN,
            .str = &param_b
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, DATATYPE_COMPATIBILITY_ERR);
      EXPECT_EQ(out_type, UNDEFINED);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, STRING);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, STRING);
   }   

   {
      token tokens[3] = {
         {
            .type = STRING_LITERAL_TOKEN,
            .str = &str_A
         },
         {
            .type = DIVISON_TOKEN
         },
         {
            .type = ID_TOKEN,
            .str = &param_b
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, DATATYPE_COMPATIBILITY_ERR);
      EXPECT_EQ(out_type, UNDEFINED);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, STRING);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, STRING);
   }   
}

TEST_F(ExpressionParserTests, TypeSemanticTests)
{
   string str_A;
   strInit(&str_A);
   strAddChar(&str_A, 'A');      
  
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
      DLInsertLast(&scoped_symtables);
      symtable_init(&scoped_symtables.Last->root_ptr);

      string param_b;
      strInit(&param_b);
      strAddChar(&param_b, 'b');      
      symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_b, INT);

      token tokens[3] = {
         {
            .type = STRING_LITERAL_TOKEN,
            .str = &str_A
         },
         {
            .type = GREATER_TOKEN
         },
         {
            .type = ID_TOKEN,
            .str = &param_b
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, DATATYPE_COMPATIBILITY_ERR);
      EXPECT_EQ(out_type, UNDEFINED);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, STRING);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, INT);
   }

   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
      DLInsertLast(&scoped_symtables);
      symtable_init(&scoped_symtables.Last->root_ptr);

      string param_b;
      strInit(&param_b);
      strAddChar(&param_b, 'b');      
      symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_b, FLOAT);

      token tokens[3] = {
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 55
         },
         {
            .type = MULTIPLICATION_TOKEN
         },
         {
            .type = ID_TOKEN,
            .str = &param_b
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, DATATYPE_COMPATIBILITY_ERR);
      EXPECT_EQ(out_type, UNDEFINED);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, INT);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, FLOAT);
   }
}

TEST_F(ExpressionParserTests, ArithmeticCombinationTests)
{       
   // +
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
      DLInsertLast(&scoped_symtables);
      symtable_init(&scoped_symtables.Last->root_ptr);

      string param_c;
      strInit(&param_c);
      strAddChar(&param_c, 'c');      
      symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_c, FLOAT);
          
      token tokens[3] = {
         {
            .type = ID_TOKEN,
            .str = &param_c
         },
         {
            .type = PLUS_TOKEN
         },
         {
            .type = DECIMAL_LITERAL_TOKEN,
            .decimal = 5.56
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, FLOAT);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, FLOAT);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, FLOAT);
      EXPECT_EQ(reduction_steps->next->next->rule, nt_plus_nt);
      EXPECT_EQ(reduction_steps->next->next->type, FLOAT);
   }

   // -
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
      DLInsertLast(&scoped_symtables);
      symtable_init(&scoped_symtables.Last->root_ptr);

      string param_c;
      strInit(&param_c);
      strAddChar(&param_c, 'c');      
      symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_c, FLOAT);
          
      token tokens[3] = {
         {
            .type = ID_TOKEN,
            .str = &param_c
         },
         {
            .type = MINUS_TOKEN
         },
         {
            .type = DECIMAL_LITERAL_TOKEN,
            .decimal = 5.56
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, FLOAT);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, FLOAT);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, FLOAT);
      EXPECT_EQ(reduction_steps->next->next->rule, nt_minus_nt);
      EXPECT_EQ(reduction_steps->next->next->type, FLOAT);
   }

   // *
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
      DLInsertLast(&scoped_symtables);
      symtable_init(&scoped_symtables.Last->root_ptr);

      string param_c;
      strInit(&param_c);
      strAddChar(&param_c, 'c');      
      symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_c, FLOAT);
          
      token tokens[3] = {
         {
            .type = ID_TOKEN,
            .str = &param_c
         },
         {
            .type = MULTIPLICATION_TOKEN
         },
         {
            .type = DECIMAL_LITERAL_TOKEN,
            .decimal = 5.56
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, FLOAT);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, FLOAT);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, FLOAT);
      EXPECT_EQ(reduction_steps->next->next->rule, nt_mul_nt);
      EXPECT_EQ(reduction_steps->next->next->type, FLOAT);
   }

   // /
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
      DLInsertLast(&scoped_symtables);
      symtable_init(&scoped_symtables.Last->root_ptr);

      string param_c;
      strInit(&param_c);
      strAddChar(&param_c, 'c');      
      symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_c, FLOAT);
          
      token tokens[3] = {
         {
            .type = ID_TOKEN,
            .str = &param_c
         },
         {
            .type = DIVISON_TOKEN
         },
         {
            .type = DECIMAL_LITERAL_TOKEN,
            .decimal = 5.56
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, FLOAT);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, FLOAT);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, FLOAT);
      EXPECT_EQ(reduction_steps->next->next->rule, nt_div_nt);
      EXPECT_EQ(reduction_steps->next->next->type, FLOAT);
   }
};

TEST_F(ExpressionParserTests, RelationalCombinationTests)
{       
   // ==
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
      DLInsertLast(&scoped_symtables);
      symtable_init(&scoped_symtables.Last->root_ptr);

      string param_c;
      strInit(&param_c);
      strAddChar(&param_c, 'c');      
      symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_c, FLOAT);
          
      token tokens[3] = {
         {
            .type = ID_TOKEN,
            .str = &param_c
         },
         {
            .type = EQUALS_TOKEN
         },
         {
            .type = DECIMAL_LITERAL_TOKEN,
            .decimal = 5.56
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, BOOL);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, FLOAT);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, FLOAT);
      EXPECT_EQ(reduction_steps->next->next->rule, nt_eq_nt);
      EXPECT_EQ(reduction_steps->next->next->type, BOOL);
   }

   // !=
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
      DLInsertLast(&scoped_symtables);
      symtable_init(&scoped_symtables.Last->root_ptr);

      string param_c;
      strInit(&param_c);
      strAddChar(&param_c, 'c');      
      symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_c, FLOAT);
          
      token tokens[3] = {
         {
            .type = ID_TOKEN,
            .str = &param_c
         },
         {
            .type = NOT_EQUALS_TOKEN
         },
         {
            .type = DECIMAL_LITERAL_TOKEN,
            .decimal = 5.56
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, BOOL);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, FLOAT);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, FLOAT);
      EXPECT_EQ(reduction_steps->next->next->rule, nt_neq_nt);
      EXPECT_EQ(reduction_steps->next->next->type, BOOL);
   }

   // <
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
      DLInsertLast(&scoped_symtables);
      symtable_init(&scoped_symtables.Last->root_ptr);

      string param_c;
      strInit(&param_c);
      strAddChar(&param_c, 'c');      
      symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_c, FLOAT);
          
      token tokens[3] = {
         {
            .type = ID_TOKEN,
            .str = &param_c
         },
         {
            .type = LESS_TOKEN
         },
         {
            .type = DECIMAL_LITERAL_TOKEN,
            .decimal = 5.56
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, BOOL);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, FLOAT);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, FLOAT);
      EXPECT_EQ(reduction_steps->next->next->rule, nt_less_nt);
      EXPECT_EQ(reduction_steps->next->next->type, BOOL);
   }

   // <= 
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
      DLInsertLast(&scoped_symtables);
      symtable_init(&scoped_symtables.Last->root_ptr);

      string param_c;
      strInit(&param_c);
      strAddChar(&param_c, 'c');      
      symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_c, FLOAT);
          
      token tokens[3] = {
         {
            .type = ID_TOKEN,
            .str = &param_c
         },
         {
            .type = LESS_EQUAL_TOKEN
         },
         {
            .type = DECIMAL_LITERAL_TOKEN,
            .decimal = 5.56
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, BOOL);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, FLOAT);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, FLOAT);
      EXPECT_EQ(reduction_steps->next->next->rule, nt_less_eq_nt);
      EXPECT_EQ(reduction_steps->next->next->type, BOOL);
   }

   // >
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
      DLInsertLast(&scoped_symtables);
      symtable_init(&scoped_symtables.Last->root_ptr);

      string param_c;
      strInit(&param_c);
      strAddChar(&param_c, 'c');      
      symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_c, FLOAT);
          
      token tokens[3] = {
         {
            .type = ID_TOKEN,
            .str = &param_c
         },
         {
            .type = GREATER_TOKEN
         },
         {
            .type = DECIMAL_LITERAL_TOKEN,
            .decimal = 5.56
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, BOOL);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, FLOAT);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, FLOAT);
      EXPECT_EQ(reduction_steps->next->next->rule, nt_more_nt);
      EXPECT_EQ(reduction_steps->next->next->type, BOOL);
   }

   // >= 
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
      DLInsertLast(&scoped_symtables);
      symtable_init(&scoped_symtables.Last->root_ptr);

      string param_c;
      strInit(&param_c);
      strAddChar(&param_c, 'c');      
      symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_c, FLOAT);
          
      token tokens[3] = {
         {
            .type = ID_TOKEN,
            .str = &param_c
         },
         {
            .type = GREATER_EQUAL_TOKEN
         },
         {
            .type = DECIMAL_LITERAL_TOKEN,
            .decimal = 5.56
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, BOOL);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, FLOAT);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, FLOAT);
      EXPECT_EQ(reduction_steps->next->next->rule, nt_more_eq_nt);
      EXPECT_EQ(reduction_steps->next->next->type, BOOL);
   }
};

TEST_F(ExpressionParserTests, BracketsTests)
{    
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
     
      token tokens[3] = {
         {
            .type = LEFT_BRACKET_TOKEN
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 5
         },
         {
            .type = RIGHT_BRACKET_TOKEN
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, INT);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, INT);      
   }

   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
      DLInsertLast(&scoped_symtables);
      symtable_init(&scoped_symtables.Last->root_ptr);

      string param_c;
      strInit(&param_c);
      strAddChar(&param_c, 'c');      
      symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_c, FLOAT);
     
      token tokens[3] = {
         {
            .type = LEFT_BRACKET_TOKEN
         },
         {
            .type = ID_TOKEN,
            .str = &param_c
         },
         {
            .type = RIGHT_BRACKET_TOKEN
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, FLOAT);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, FLOAT);      
      EXPECT_EQ(reduction_steps->next->rule, lbrack_nt_rbrack);
      EXPECT_EQ(reduction_steps->next->type, FLOAT);     
   }

   // (((i)))
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
     
      token tokens[7] = {
         {
            .type = LEFT_BRACKET_TOKEN
         },
         {
            .type = LEFT_BRACKET_TOKEN
         },
         {
            .type = LEFT_BRACKET_TOKEN
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 5
         },
         {
            .type = RIGHT_BRACKET_TOKEN
         },
         {
            .type = RIGHT_BRACKET_TOKEN
         },
         {
            .type = RIGHT_BRACKET_TOKEN
         }      
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 7, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, INT);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, INT); 
      EXPECT_EQ(reduction_steps->next->rule, lbrack_nt_rbrack);
      EXPECT_EQ(reduction_steps->next->type, INT);    
      EXPECT_EQ(reduction_steps->next->next->rule, lbrack_nt_rbrack);
      EXPECT_EQ(reduction_steps->next->next->type, INT);    
      EXPECT_EQ(reduction_steps->next->next->next->rule, lbrack_nt_rbrack);
      EXPECT_EQ(reduction_steps->next->next->next->type, INT);
   }

   // (i))
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
     
      token tokens[4] = {
         {
            .type = LEFT_BRACKET_TOKEN
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 5
         },
         {
            .type = RIGHT_BRACKET_TOKEN
         },
         {
            .type = RIGHT_BRACKET_TOKEN
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 4, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, SYNTAX_ERR);
      EXPECT_EQ(out_type, UNDEFINED);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, INT);      
   }

   // ((i)
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
     
      token tokens[4] = {
          {
            .type = LEFT_BRACKET_TOKEN
         },
         {
            .type = LEFT_BRACKET_TOKEN
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 5
         },
         {
            .type = RIGHT_BRACKET_TOKEN
         }        
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 4, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, SYNTAX_ERR);
      EXPECT_EQ(out_type, UNDEFINED);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, INT);      
      EXPECT_EQ(reduction_steps->next->rule, lbrack_nt_rbrack);
      EXPECT_EQ(reduction_steps->next->type, INT);  
   }

    // )(i)
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
     
      token tokens[4] = {
          {
            .type = RIGHT_BRACKET_TOKEN
         },
         {
            .type = LEFT_BRACKET_TOKEN
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 5
         },
         {
            .type = RIGHT_BRACKET_TOKEN
         }        
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 4, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, SYNTAX_ERR);
      EXPECT_EQ(out_type, UNDEFINED);
   }  
}

TEST_F(ExpressionParserTests, PrecedenceTests)
{    
   // a + b * c
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
     
      token tokens[5] = {
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 5
         },
         {
            .type = PLUS_TOKEN,
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 10
         },
         {
            .type = MULTIPLICATION_TOKEN,
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 15
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 5, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, INT);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, INT);  
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, INT); 
      EXPECT_EQ(reduction_steps->next->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->next->type, INT); 
      EXPECT_EQ(reduction_steps->next->next->next->rule, nt_mul_nt);
      EXPECT_EQ(reduction_steps->next->next->next->type, INT); 
      EXPECT_EQ(reduction_steps->next->next->next->next->rule, nt_plus_nt);
      EXPECT_EQ(reduction_steps->next->next->next->next->type, INT);     
   }

   // a * b + c
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
     
      token tokens[5] = {
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 5
         },
         {
            .type = MULTIPLICATION_TOKEN,
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 10
         },
         {
            .type = PLUS_TOKEN,
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 15
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 5, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, INT);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, INT);  
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, INT); 
      EXPECT_EQ(reduction_steps->next->next->rule, nt_mul_nt);
      EXPECT_EQ(reduction_steps->next->next->type, INT); 
      EXPECT_EQ(reduction_steps->next->next->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->next->next->type, INT); 
      EXPECT_EQ(reduction_steps->next->next->next->next->rule, nt_plus_nt);
      EXPECT_EQ(reduction_steps->next->next->next->next->type, INT);     
   }

   // (a + b) * c
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
     
      token tokens[7] = {
         {
            .type = LEFT_BRACKET_TOKEN,
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 5
         },
         {
            .type = PLUS_TOKEN,
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 10
         },
         {
            .type = RIGHT_BRACKET_TOKEN,
         },
         {
            .type = MULTIPLICATION_TOKEN,
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 15
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 7, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, INT);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, INT);  
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, INT); 
      EXPECT_EQ(reduction_steps->next->next->rule, nt_plus_nt);
      EXPECT_EQ(reduction_steps->next->next->type, INT); 
      EXPECT_EQ(reduction_steps->next->next->next->rule, lbrack_nt_rbrack);
      EXPECT_EQ(reduction_steps->next->next->next->type, INT); 
      EXPECT_EQ(reduction_steps->next->next->next->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->next->next->next->type, INT);  
      EXPECT_EQ(reduction_steps->next->next->next->next->next->rule, nt_mul_nt);
      EXPECT_EQ(reduction_steps->next->next->next->next->next->type, INT);     
   }
}

TEST_F(ExpressionParserTests, ExtendeExpressionsTests)
{ 
   // ((a + b) * c / d - e) == f
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
     
      token tokens[15] = {
         {
            .type = LEFT_BRACKET_TOKEN,
         },
         {
            .type = LEFT_BRACKET_TOKEN,
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 5
         },
         {
            .type = PLUS_TOKEN,
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 10
         },
         {
            .type = RIGHT_BRACKET_TOKEN,
         },
         {
            .type = MULTIPLICATION_TOKEN,
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 15
         },
         {
            .type = DIVISON_TOKEN,
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 556
         },
         {
            .type = MINUS_TOKEN,
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 755857
         },
         {
            .type = RIGHT_BRACKET_TOKEN,
         },
         {
            .type = EQUALS_TOKEN,
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 58
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 15, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, BOOL);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, INT);  
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, INT); 
      EXPECT_EQ(reduction_steps->next->next->rule, nt_plus_nt);
      EXPECT_EQ(reduction_steps->next->next->type, INT); 
      EXPECT_EQ(reduction_steps->next->next->next->rule, lbrack_nt_rbrack);
      EXPECT_EQ(reduction_steps->next->next->next->type, INT); 
      EXPECT_EQ(reduction_steps->next->next->next->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->next->next->next->type, INT);  
      EXPECT_EQ(reduction_steps->next->next->next->next->next->rule, nt_mul_nt);
      EXPECT_EQ(reduction_steps->next->next->next->next->next->type, INT);     
      EXPECT_EQ(reduction_steps->next->next->next->next->next->rule, nt_mul_nt);
      EXPECT_EQ(reduction_steps->next->next->next->next->next->type, INT);     
      EXPECT_EQ(reduction_steps->next->next->next->next->next->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->next->next->next->next->next->type, INT);     
      EXPECT_EQ(reduction_steps->next->next->next->next->next->next->next->rule, nt_div_nt);
      EXPECT_EQ(reduction_steps->next->next->next->next->next->next->next->type, INT);     
      EXPECT_EQ(reduction_steps->next->next->next->next->next->next->next->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->next->next->next->next->next->next->next->type, INT);    
      EXPECT_EQ(reduction_steps->next->next->next->next->next->next->next->next->next->rule, nt_minus_nt);
      EXPECT_EQ(reduction_steps->next->next->next->next->next->next->next->next->next->type, INT);   
      EXPECT_EQ(reduction_steps->next->next->next->next->next->next->next->next->next->next->rule, lbrack_nt_rbrack);
      EXPECT_EQ(reduction_steps->next->next->next->next->next->next->next->next->next->next->type, INT);     
      EXPECT_EQ(reduction_steps->next->next->next->next->next->next->next->next->next->next->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->next->next->next->next->next->next->next->next->next->next->type, INT);   
      EXPECT_EQ(reduction_steps->next->next->next->next->next->next->next->next->next->next->next->next->rule, nt_eq_nt);
      EXPECT_EQ(reduction_steps->next->next->next->next->next->next->next->next->next->next->next->next->type, BOOL);   
   }
}

TEST_F(ExpressionParserTests, ZeroDivisionTestInt)
{    
   // c / 0
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
      DLInsertLast(&scoped_symtables);
      symtable_init(&scoped_symtables.Last->root_ptr);

      string param_c;
      strInit(&param_c);
      strAddChar(&param_c, 'c');      
      symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_c, INT);
          
      token tokens[3] = {
         {
            .type = ID_TOKEN,
            .str = &param_c
         },
         {
            .type = DIVISON_TOKEN
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 0
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, DIVIDE_ZERO_ERR);
      EXPECT_EQ(out_type, UNDEFINED);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, INT);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, INT);
   }

   // c / ((8 + 8) * 8 / 16 - 8)
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
      DLInsertLast(&scoped_symtables);
      symtable_init(&scoped_symtables.Last->root_ptr);

      string param_c;
      strInit(&param_c);
      strAddChar(&param_c, 'c');      
      symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_c, INT);
          
      token tokens[15] = {
         {
            .type = ID_TOKEN,
            .str = &param_c
         },
         {
            .type = DIVISON_TOKEN
         },
         {
            .type = LEFT_BRACKET_TOKEN,
         },
         {
            .type = LEFT_BRACKET_TOKEN,
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 8
         },
         {
            .type = PLUS_TOKEN
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 8
         },
         {
            .type = RIGHT_BRACKET_TOKEN,
         },
         {
            .type = MULTIPLICATION_TOKEN
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 8
         },
         {
            .type = DIVISON_TOKEN
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 16
         },
         {
            .type = MINUS_TOKEN
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 8
         },
         {
            .type = RIGHT_BRACKET_TOKEN
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 15, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, DIVIDE_ZERO_ERR);
      EXPECT_EQ(out_type, UNDEFINED);
   }

   // c / ((8 + 8) * 8 / 16 - 8)
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
      DLInsertLast(&scoped_symtables);
      symtable_init(&scoped_symtables.Last->root_ptr);

      string param_c;
      strInit(&param_c);
      strAddChar(&param_c, 'c');      
      symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_c, INT);
          
      token tokens[15] = {
         {
            .type = ID_TOKEN,
            .str = &param_c
         },
         {
            .type = DIVISON_TOKEN
         },
         {
            .type = LEFT_BRACKET_TOKEN,
         },
         {
            .type = LEFT_BRACKET_TOKEN,
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 8
         },
         {
            .type = PLUS_TOKEN
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 8
         },
         {
            .type = RIGHT_BRACKET_TOKEN,
         },
         {
            .type = MULTIPLICATION_TOKEN
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 8
         },
         {
            .type = DIVISON_TOKEN
         },
         {
            .type = INTEGER_LITERAL_TOKEN,
            .integer = 16
         },
         {
            .type = MINUS_TOKEN
         },
         {
            .type = ID_TOKEN,
            .str = &param_c
         },
         {
            .type = RIGHT_BRACKET_TOKEN
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 15, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, INT);
   }
}

TEST_F(ExpressionParserTests, ZeroDivisionTestFloat)
{    
   // c / 0
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
      DLInsertLast(&scoped_symtables);
      symtable_init(&scoped_symtables.Last->root_ptr);

      string param_c;
      strInit(&param_c);
      strAddChar(&param_c, 'c');      
      symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_c, FLOAT);
          
      token tokens[3] = {
         {
            .type = ID_TOKEN,
            .str = &param_c
         },
         {
            .type = DIVISON_TOKEN
         },
         {
            .type = DECIMAL_LITERAL_TOKEN,
            .decimal = 0
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 3, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, DIVIDE_ZERO_ERR);
      EXPECT_EQ(out_type, UNDEFINED);
      EXPECT_EQ(reduction_steps->rule, operand);
      EXPECT_EQ(reduction_steps->type, FLOAT);
      EXPECT_EQ(reduction_steps->next->rule, operand);
      EXPECT_EQ(reduction_steps->next->type, FLOAT);
   }

   // c / ((4 + 3) * 8 / 16 - 3.5)
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
      DLInsertLast(&scoped_symtables);
      symtable_init(&scoped_symtables.Last->root_ptr);

      string param_c;
      strInit(&param_c);
      strAddChar(&param_c, 'c');      
      symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_c, FLOAT);
          
      token tokens[15] = {
         {
            .type = ID_TOKEN,
            .str = &param_c
         },
         {
            .type = DIVISON_TOKEN
         },
         {
            .type = LEFT_BRACKET_TOKEN,
         },
         {
            .type = LEFT_BRACKET_TOKEN,
         },
         {
            .type = DECIMAL_LITERAL_TOKEN,
            .decimal = 4.0
         },
         {
            .type = PLUS_TOKEN
         },
         {
            .type = DECIMAL_LITERAL_TOKEN,
            .decimal = 3.0
         },
         {
            .type = RIGHT_BRACKET_TOKEN,
         },
         {
            .type = MULTIPLICATION_TOKEN
         },
         {
            .type = DECIMAL_LITERAL_TOKEN,
            .decimal = 8.0
         },
         {
            .type = DIVISON_TOKEN
         },
         {
            .type = DECIMAL_LITERAL_TOKEN,
            .decimal = 16.0
         },
         {
            .type = MINUS_TOKEN
         },
         {
            .type = DECIMAL_LITERAL_TOKEN,
            .decimal = 3.5
         },
         {
            .type = RIGHT_BRACKET_TOKEN
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 15, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, DIVIDE_ZERO_ERR);
      EXPECT_EQ(out_type, UNDEFINED);
   }

   // c / ((4 + 3) * 8 / c - 3.5)
   {
      tDLList scoped_symtables;
      DLInitList(&scoped_symtables);
      DLInsertLast(&scoped_symtables);
      symtable_init(&scoped_symtables.Last->root_ptr);

      string param_c;
      strInit(&param_c);
      strAddChar(&param_c, 'c');      
      symtable_insert_node_var(&scoped_symtables.Last->root_ptr, &param_c, FLOAT);
          
      token tokens[15] = {
         {
            .type = ID_TOKEN,
            .str = &param_c
         },
         {
            .type = DIVISON_TOKEN
         },
         {
            .type = LEFT_BRACKET_TOKEN,
         },
         {
            .type = LEFT_BRACKET_TOKEN,
         },
         {
            .type = DECIMAL_LITERAL_TOKEN,
            .decimal = 4.0
         },
         {
            .type = PLUS_TOKEN
         },
         {
            .type = DECIMAL_LITERAL_TOKEN,
            .decimal = 3.0
         },
         {
            .type = RIGHT_BRACKET_TOKEN,
         },
         {
            .type = MULTIPLICATION_TOKEN
         },
         {
            .type = ID_TOKEN,
            .str = &param_c
         },
         {
            .type = DIVISON_TOKEN
         },
         {
            .type = DECIMAL_LITERAL_TOKEN,
            .decimal = 16.0
         },
         {
            .type = MINUS_TOKEN
         },
         {
            .type = DECIMAL_LITERAL_TOKEN,
            .decimal = 3.5
         },
         {
            .type = RIGHT_BRACKET_TOKEN
         }
      };

      varType out_type;
      instrumented_node *reduction_steps = NULL;
      int expr_parser_output = parse_instrumented_expression(&scoped_symtables,
         tokens, 15, &out_type, &reduction_steps);

      EXPECT_EQ(expr_parser_output, OK);
      EXPECT_EQ(out_type, FLOAT);
   }
}