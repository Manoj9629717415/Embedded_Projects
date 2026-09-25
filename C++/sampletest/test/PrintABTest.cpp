#include <gtest/gtest.h>
#include "PrintAB.h"
#include <thread>
#include <chrono>


class PrintABTest : public ::testing::Test{

public:

    void SetUp() override
    {

    }

    void TearDown() override
    {

    }
protected:
    Print t_print;
};


TEST_F(PrintABTest,checkshutdown)
{
    std::thread thA(&Print::printA,&t_print);
    std::thread thB(&Print::printB,&t_print);

    t_print.shutdown();

    thA.join();
    thB.join();

    SUCCEED();
}

TEST_F(PrintABTest,checkdataswing)
{
    testing::internal::CaptureStdout();

    std::thread thA(&Print::printA,&t_print);
    std::thread thB(&Print::printB,&t_print);


    std::this_thread::sleep_for(std::chrono::microseconds(300));

    t_print.shutdown();

    thA.join();
    thB.join();

    std::string out = testing::internal::GetCapturedStdout();

    EXPECT_NE(out.find("ab"),std::string::npos);
}