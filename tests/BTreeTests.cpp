
extern "C"
{
#include "../src/symtable.h"
#include "../src/error_codes.h"
#include "../src/str.h"
}
#include <gtest/gtest.h>

class BTreeTests : public ::testing::Test
{
protected:
    symbol_node *rootptr;
    string root;
    string left;
    string right;
    string left_right;
    string left_left;
    string right_right;
    string right_left;
    void SetUp()
    {
        symtable_init(&rootptr);
        strInit(&root);
        strInit(&left);
        strInit(&right);
        strInit(&left_right);
        strInit(&left_left);
        strInit(&right_right);
        strInit(&right_left);
        strAddChar(&root, 'f');
        strAddChar(&left, 'b');
        strAddChar(&right, 'h');
        strAddChar(&left_right, 'd');
        strAddChar(&left_left, 'a');
        strAddChar(&right_right, 'z');
        strAddChar(&right_left, 'g');
    }
    void TearDown()
    {
        strFree(&root);
        strFree(&left);
        strFree(&right);
        strFree(&left_right);
        strFree(&left_left);
        strFree(&right_right);
        strFree(&right_left);
        symtable_dispose_tree(&rootptr);
    }
};

TEST_F(BTreeTests, VarTreeTest)
{
    //insert values

    EXPECT_EQ(symtable_insert_node_var(&rootptr, &root, INT), OK);
    EXPECT_EQ(symtable_insert_node_var(&rootptr, &left, INT), OK);
    EXPECT_EQ(symtable_insert_node_var(&rootptr, &right, STRING), OK);
    EXPECT_EQ(symtable_insert_node_var(&rootptr, &right_left, STRING), OK);
    EXPECT_EQ(symtable_insert_node_var(&rootptr, &right_right, STRING), OK);
    EXPECT_EQ(symtable_insert_node_var(&rootptr, &left_left, STRING), OK);
    EXPECT_EQ(symtable_insert_node_var(&rootptr, &left_right, STRING), OK);
    EXPECT_EQ(strCmpString(rootptr->name, &root), 0);
    EXPECT_EQ(strCmpString(rootptr->l_ptr->name, &left), 0);
    EXPECT_EQ(strCmpString(rootptr->r_ptr->name, &right), 0);
    EXPECT_EQ(strCmpString(rootptr->l_ptr->l_ptr->name, &left_left), 0);
    EXPECT_EQ(strCmpString(rootptr->l_ptr->r_ptr->name, &left_right), 0);
    EXPECT_EQ(strCmpString(rootptr->r_ptr->r_ptr->name, &right_right), 0);
    EXPECT_EQ(strCmpString(rootptr->r_ptr->l_ptr->name, &right_left), 0);

    //search for values
    symbol_node *found = NULL;
    string key1;
    strInit(&key1);
    strAddChar(&key1, 'a');
    strAddChar(&key1, 'b');
    EXPECT_FALSE(symtable_search(&rootptr, &key1, &found));
    EXPECT_EQ(found, nullptr);

    strClear(&key1);
    strAddChar(&key1, 'b');
    EXPECT_TRUE(symtable_search(&rootptr, &key1, &found));
    EXPECT_EQ(strCmpString(found->name, &key1), 0);
    EXPECT_EQ(found->type, var);
    EXPECT_EQ(((symbol_variable *)found->data)->var_type, INT);

    strClear(&key1);
    strAddChar(&key1, 'g');
    EXPECT_TRUE(symtable_search(&rootptr, &key1, &found));
    EXPECT_EQ(strCmpString(found->name, &key1), 0);
    EXPECT_EQ(found->type, var);
    EXPECT_EQ(((symbol_variable *)found->data)->var_type, STRING);

    //dispose tree test
    EXPECT_EQ(symtable_dispose_tree(&rootptr), OK);
    EXPECT_EQ(rootptr, nullptr);
    strFree(&key1);
}

TEST_F(BTreeTests, FuncTreeTest)
{

    varType returns[] = {INT, FLOAT};
    varType params[] = {INT, STRING, FLOAT};
    varType returns2[] = {INT};
    varType params2[] = {STRING};
    EXPECT_EQ(symtable_insert_node_func(&rootptr, &root, 2, returns, 3, params, true), OK);
    EXPECT_EQ(symtable_insert_node_func(&rootptr, &left, 1, returns2, 1, params2, true), OK);
    EXPECT_EQ(symtable_insert_node_func(&rootptr, &right, 0, NULL, 0, NULL, true), OK);

    EXPECT_EQ(symtable_insert_node_func(&rootptr, &left_right, 1, returns2, 3, params, true), OK);
    EXPECT_FALSE(symtable_contains_undef_func(&rootptr));
    EXPECT_EQ(symtable_insert_node_func(&rootptr, &left_left, 0, NULL, 1, params2, false), OK);
    EXPECT_TRUE(symtable_contains_undef_func(&rootptr));
    EXPECT_EQ(symtable_insert_node_func(&rootptr, &right_right, 1, returns2, 1, params2, true), OK);
    EXPECT_EQ(symtable_insert_node_func(&rootptr, &right_left, 2, returns, 1, params2, true), OK);
    string non_existing;
    strInit(&non_existing);
    strAddChar(&non_existing, 'a');
    strAddChar(&non_existing, 's');

    symbol_node *found = nullptr;
    EXPECT_FALSE(symtable_search(&rootptr, &non_existing, &found));
    EXPECT_EQ(found, nullptr);
    strFree(&non_existing);
    EXPECT_TRUE(symtable_search(&rootptr, &left_right, &found));
    EXPECT_EQ(strCmpString(found->name, &left_right), 0);
    EXPECT_EQ(found->type, func);
    symbol_function *data = (symbol_function *)found->data;
    EXPECT_TRUE(data->defined);
    EXPECT_EQ(data->return_types_count, 1);
    EXPECT_EQ(data->par_count, 3);
    for (int i = 0; i < data->return_types_count; i++)
    {
        EXPECT_EQ(data->return_types[i], returns2[i]);
    }
    for (int i = 0; i < data->par_count; i++)
    {
        EXPECT_EQ(data->parameters[i], params[i]);
    }
    EXPECT_EQ(symtable_dispose_tree(&rootptr), OK);
    EXPECT_EQ(rootptr, nullptr);
}