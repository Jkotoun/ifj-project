
extern "C" {
#include "../src/symtable.h"
#include "../src/error_codes.h"
}
#include <gtest/gtest.h>

class BTreeTests : public ::testing::Test
{
   protected:
    node *rootptr = NULL;  
   void SetUp() 
   {
       init(&rootptr);
   }
    void TearDown() 
   {
     
   }
};


TEST_F(BTreeTests, VarTreeTest)
{
    char root[] = "f";
    char left[] = "b";
    char right[] = "h";
    char left_right[] = "d";
    char left_left[] = "a";
    char right_right[] = "z";
    char right_left[] = "g";
    EXPECT_EQ(insert_node_var(&rootptr,root, INT), OK);
    EXPECT_EQ(insert_node_var(&rootptr,left,STRING), OK);
    EXPECT_EQ(insert_node_var(&rootptr, right, STRING), OK);
    EXPECT_EQ(insert_node_var(&rootptr, right_left, STRING), OK);
    EXPECT_EQ(insert_node_var(&rootptr, right_right, STRING), OK);
    EXPECT_EQ(insert_node_var(&rootptr, left_left, STRING), OK);
    EXPECT_EQ(insert_node_var(&rootptr, left_right, STRING), OK);
    EXPECT_EQ(rootptr->name, root);
    EXPECT_EQ(rootptr->l_ptr->name, left);
    EXPECT_EQ(rootptr->r_ptr->name, right);
    EXPECT_EQ(rootptr->l_ptr->l_ptr->name, left_left);
    EXPECT_EQ(rootptr->l_ptr->r_ptr->name, left_right);
    EXPECT_EQ(rootptr->r_ptr->r_ptr->name, right_right);
    EXPECT_EQ(rootptr->r_ptr->l_ptr->name, right_left);
    EXPECT_EQ(dispose_tree(&rootptr), OK);
    EXPECT_EQ(rootptr, nullptr);

    
}