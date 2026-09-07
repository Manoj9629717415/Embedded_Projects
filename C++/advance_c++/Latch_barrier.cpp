#include <iostream>
#include <latch>
#include <barrier>
#include <thread>
#include <chrono>
#include <vector>
#include <string>

void sensor_init(std::string sensorname,int id,std::latch& l)
{
    std::cout<<"Sensor "<<sensorname<<" initializing \n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500 + (id*1000)));
    std::cout<<"Sensor "<<sensorname<<" initialized \n";
    l.count_down();
}

void Process_values(std::string sensorname,int id,std::barrier<>& sync)
{
    int round = 0;
    while(round < 3)
    {
        std::cout<<" thread id "<<id<<" sensor acquisition "<<" round "<<round<<std::endl;
        std::this_thread::sleep_for(
            std::chrono::milliseconds(500 + id * 100));
            sync.arrive_and_wait();
        std::cout<<" thread id "<<id<<" Data Processing "<<" round "<<round<<std::endl;
        std::this_thread::sleep_for(
            std::chrono::milliseconds(300 + id * 100));
            sync.arrive_and_wait();
        std::cout<<" thread id "<<id<<" Control calculation "<<" round "<<round<<std::endl;
        std::this_thread::sleep_for(
            std::chrono::milliseconds(200 + id * 100));
        sync.arrive_and_wait();
        round++;
    }

}

int main()
{
    std::latch l(4);
    std::vector<std::string> sensors{"Engine","Transmission","Brake","Diagnostics"};
    std::vector<std::jthread> ths;

    for(int i=0;i<4;i++)
    {
        ths.emplace_back(sensor_init,sensors[i],i,std::ref(l));
    }
    l.wait();
    std::cout<<"All Sensors Initialized \n";

    std::barrier sync(4);
    std::vector<std::jthread> ba_ths;

    for(int i=0;i<4;i++)
    {
        ba_ths.emplace_back(Process_values,sensors[i],i,std::ref(sync));
    }
}