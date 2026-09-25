#include "BoundedQueue_Dynamic.h"
#include <gtest/gtest.h>
#include <thread>


class BoundedQueue_DaynamicTest : public ::testing::Test{
public:
    // void Setup() override{

    // }

    // void Teardown() override{

    // }

protected:
    BoundedQueue_Dynamic<int> m_bq_dyn{10};

};

TEST_F(BoundedQueue_DaynamicTest,checkshudown)
{

    std::thread th1([this](){
        this->m_bq_dyn.push(1);
    });

    std::thread th2([this](){
        int data;
        this->m_bq_dyn.pop(data);
    });

    m_bq_dyn.shutdown();

    th1.join();
    th2.join();

    SUCCEED();

}