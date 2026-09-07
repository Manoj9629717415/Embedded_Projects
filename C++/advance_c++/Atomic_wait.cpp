#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>


std::atomic<int> diag_counter{0};
std::atomic<bool> shutdown{false};
std::atomic<int> count{0};

void consumer()
{
        while(true)
        {

            int expected = 0;

            while(diag_counter.load() == 0)
            {
                if(shutdown.load()) return;
                diag_counter.wait(expected);
            }

            if(shutdown.load()) break;

            diag_counter.fetch_sub(1);
            int old = count.fetch_add(1);
            
            std::cout<<"thread id"<<std::this_thread::get_id()<<" count "<<count<<" \n";

            if (old >= 20)
            {
                count.fetch_sub(1);
                return;
            }

             if(old + 1 == 20)
            {
                bool expected_bool = false;
                shutdown.store(true);
                shutdown.notify_all();
                diag_counter.notify_all();
                break;
            }
        }
        std::cout<<"Consumer Exit \n";

}

void producer(std::stop_token token)
{
    while(!token.stop_requested())
    {
        diag_counter.fetch_add(1);
        diag_counter.notify_all();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    std::cout<<"Producer Exit \n";

}

int main()
{
    std::vector<std::thread> consumer_ths;
    std::vector<std::jthread> producer_ths;
    for(int i=0;i<3;i++)
    {
        consumer_ths.emplace_back(consumer);
    }

    for(int i=0;i<2;i++)
    {
        producer_ths.emplace_back([](std::stop_token token){
            producer(token);
        });
    }
    bool expcted_shut = false;

    while(shutdown.load() == false)
    {
        shutdown.wait(expcted_shut);
    }

    for(auto& th : producer_ths)
    {
        th.request_stop();
    }

    for(auto& th : consumer_ths)
    {
        th.join();
    }


}