#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstring>
#include <atomic>
#include <chrono>

#include "PrintAB.h"


void Print::printA()
{
    while(true)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait(lock,[this](){ 
            return m_data == 'b' || m_shutdown;
        });
    

        if(m_shutdown){
            break;
        }

        m_data = 'a';
        std::cout<<m_data;
        lock.unlock();
        m_cv.notify_one();
    }
    std::cout<<"thread A shutdown \n";
}

void Print::printB()
{
    while(true)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait(lock,[this](){
            return m_data == 'a' || m_shutdown;
        });

        if(m_shutdown) break;

        m_data = 'b';
        std::cout<<m_data<<" \t";
        lock.unlock();
        m_cv.notify_one();
    }
    std::cout<<"thread B shutdown \n";

}

void Print::shutdown()
{
    m_shutdown.store(true);
    m_cv.notify_all();
}

char Print::getData()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_data;
}


// int main()
// {
//     Print print_obj;

//     std::thread threadA([&print_obj](){
//         print_obj.printA();
//     });

//     std::thread threadB([&print_obj](){
//         print_obj.printB();
//     });

//     std::this_thread::sleep_for(std::chrono::seconds(3));

//     print_obj.shutdown();

//     threadA.join();

//     threadB.join();

// }