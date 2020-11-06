
extern "C"
{
#include "../src/dl_list.h"
#include "../src/symtable.h"
#include "../src/error_codes.h"
}
#include <gtest/gtest.h>

class ListTests : public ::testing::Test
{
protected:
    tDLList list;
    void SetUp()
    {
        DLInitList(&list);
    }
    void TearDown()
    {
        DLDisposeList(&list);
    }
};

TEST_F(ListTests, BasicTests)
{
    //create trees for tables
    char name1[] = "b";
    char name2[] = "a";

    //insert tables with trees to list
    EXPECT_EQ(DLInsertLast(&list), OK);
    EXPECT_EQ(insert_node_var(&(list.Last->root_ptr), name1, INT), OK);
    EXPECT_EQ(DLInsertLast(&list), OK);
    EXPECT_EQ(insert_node_var(&(list.Last->root_ptr), name2, STRING), OK);
    EXPECT_EQ(DLInsertLast(&list), OK);
    EXPECT_EQ(insert_node_var(&(list.Last->root_ptr), name1, INT), OK);

    //test of right pointers and scope indexing
    EXPECT_EQ(strcmp(list.First->root_ptr->name, name1), 0);
    EXPECT_EQ(list.First->scope_index, 0);
    EXPECT_EQ(strcmp(list.First->next_table->root_ptr->name, name2), 0);
    EXPECT_EQ(list.First->next_table->scope_index, 1);
    EXPECT_EQ(strcmp(list.First->next_table->next_table->root_ptr->name, name1), 0);
    EXPECT_EQ(list.Last->scope_index, 2);
    EXPECT_EQ(list.Last->next_table, nullptr);
    //test of left pointers
    EXPECT_EQ(strcmp(list.Last->root_ptr->name, name1), 0);
    EXPECT_EQ(strcmp(list.Last->prev_table->root_ptr->name, name2), 0);
    EXPECT_EQ(strcmp(list.Last->prev_table->prev_table->root_ptr->name, name1), 0);
    EXPECT_EQ(list.First->prev_table, nullptr);
    //item delete
    EXPECT_EQ(DLDeleteLast(&list), OK);
    EXPECT_EQ(list.Last->scope_index, 1);
    EXPECT_EQ(list.Last, list.First->next_table);
    //dispose list

    EXPECT_EQ(DLDisposeList(&list), OK);
    EXPECT_EQ(list.First, nullptr);
    EXPECT_EQ(list.Last, nullptr);
}