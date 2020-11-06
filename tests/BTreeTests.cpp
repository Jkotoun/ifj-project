
extern "C" {
#include "../src/symtable.h"
#include "../src/error_codes.h"
}
#include <gtest/gtest.h>

class BTreeTests : public ::testing::Test
{
   protected:
    node *rootptr;  
   void SetUp() 
   {
       init(&rootptr);
   }
    void TearDown() 
   {
      dispose_tree(&rootptr);
   }
};


TEST_F(BTreeTests, VarTreeTest)
{
    //insert values
    char root[] = "f";
    char left[] = "b";
    char right[] = "h";
    char left_right[] = "d";
    char left_left[] = "a";
    char right_right[] = "z";
    char right_left[] = "g";
    EXPECT_EQ(insert_node_var(&rootptr,root, INT), OK);
    EXPECT_EQ(insert_node_var(&rootptr,left,INT), OK);
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

    //search for values
    node* found = NULL;
    char key1[] = "asdqwe";
    EXPECT_FALSE(search(&rootptr, key1, &found));
    EXPECT_EQ(found, nullptr);
    
    char key2[] = "b";
    EXPECT_TRUE(search(&rootptr, key2, &found));
    EXPECT_EQ(strcmp(found->name,key2), 0);
    EXPECT_EQ(found->type, var);
    EXPECT_EQ(((symbol_variable*)found->data)->var_type, INT);

    char key3[] = "g";
    EXPECT_TRUE(search(&rootptr, key3, &found));
    EXPECT_EQ(strcmp(found->name,key3), 0);
    EXPECT_EQ(found->type, var);
    EXPECT_EQ(((symbol_variable*)found->data)->var_type, STRING);

    //dispose tree test
    EXPECT_EQ(dispose_tree(&rootptr), OK);
    EXPECT_EQ(rootptr, nullptr);   
}


TEST_F(BTreeTests, FuncTreeTest)
{
      //insert values
    char root[] = "f";
    char left[] = "b";
    char right[] = "h";
    char left_right[] = "d";
    char left_left[] = "a";
    char right_right[] = "z";
    char right_left[] = "g";
    varType returns[] = {INT, FLOAT};
    varType params[] = {INT, STRING, FLOAT};
    varType returns2[] = {INT};
    varType params2[] = {STRING};
    EXPECT_EQ(insert_node_func(&rootptr,root,2,returns,3,params,true), OK);
    EXPECT_EQ(insert_node_func(&rootptr,left,1,returns2,1,params2,true), OK);
    EXPECT_EQ(insert_node_func(&rootptr,right,0,NULL,0,NULL,true), OK);

    EXPECT_EQ(insert_node_func(&rootptr,left_right,1,returns2,3,params,true), OK);

    EXPECT_EQ(insert_node_func(&rootptr,left_left,0,NULL,1,params2,false), OK);
    EXPECT_EQ(insert_node_func(&rootptr,right_right,1,returns2,1,params2,true), OK);
    EXPECT_EQ(insert_node_func(&rootptr,right_left,2,returns,1,params2,true), OK);
    char non_existing[] = "asdf";
    node* found = nullptr;
    EXPECT_FALSE(search(&rootptr,non_existing, &found));
    EXPECT_EQ(found, nullptr);

    EXPECT_TRUE(search(&rootptr, left_right, &found));
    EXPECT_EQ(strcmp(found->name, left_right), 0);
    EXPECT_EQ(found->type, func);
    symbol_function* data = (symbol_function*)found->data;
    EXPECT_TRUE(data->defined);
    EXPECT_EQ(data->return_types_count, 1);
    EXPECT_EQ(data->par_count, 3);
    for(int i = 0;i<data->return_types_count;i++)
    {
        EXPECT_EQ(data->return_types[i], returns2[i]);
    }
    for(int i = 0;i<data->par_count;i++)
    {
        EXPECT_EQ(data->parameters[i], params[i]);
    }
    EXPECT_EQ(dispose_tree(&rootptr), OK);
    EXPECT_EQ(rootptr, nullptr);
    


    
    

    
}