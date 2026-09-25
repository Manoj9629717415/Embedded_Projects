#include <gtest/gtest.h>
#include "CanMessage_hash_table.h"


class CanMessageStoreTest : public ::testing::Test{

    void SetUp() override{

    }

    void TearDown() override{

    }

protected:
    CanMessageStore_Hashtable table;
};

TEST_F(CanMessageStoreTest,AddMessage)
{
    CanMessage msg{1001,{10,20,30,40,50,60,70,80}};


    ASSERT_TRUE(table.add(msg));

    CanMessage msg1{};
  
    ASSERT_TRUE(table.get(1001,msg1));

    EXPECT_EQ(msg1.id,1001);

    EXPECT_EQ(msg1.data,msg.data);

}

TEST_F(CanMessageStoreTest,getMessageNegative){
    CanMessage msg{};

    ASSERT_FALSE(table.get(2000,msg));

}

TEST_F(CanMessageStoreTest,addDuplicateMessage){

    CanMessage msg{1001,{10,20,30,40,50,60,70,80}};

    ASSERT_TRUE(table.add(msg));

    CanMessage msg1{1001,{100,200,5,10,55,65,75,85}};

    ASSERT_TRUE(table.add(msg1));

    CanMessage msg2{};
    ASSERT_TRUE(table.get(1001,msg2));

    EXPECT_EQ(msg2.id,1001);

    EXPECT_EQ(msg2.data,msg1.data);

    EXPECT_NE(msg2.data,msg.data);

}