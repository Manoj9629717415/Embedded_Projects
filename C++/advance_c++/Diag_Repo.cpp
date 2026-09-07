#include <iostream>
#include <vector>
#include <ranges>
#include <algorithm>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <queue>
#include <atomic>
#include <chrono>
#include <functional>
#include <shared_mutex>
#include <random>
#include <memory>


struct Diagnostic{
    int id;
    int severity;
    std::string component;
    std::string message;
};

class DiagnosticRepository{
public:
    DiagnosticRepository():m_snapshot(std::make_shared<const std::vector<Diagnostic>>(std::initializer_list<Diagnostic>{{101, 2, "Engine", "Oil temperature normal"},
        {102, 5, "Engine", "Engine overheating"},
        {103, 3, "Brake", "Brake pad wear"},
        {104, 4, "Transmission", "Transmission temperature high"},
        {105, 1, "Battery", "Battery voltage normal"},
        {106, 5, "Brake", "Brake pressure failure"},
        {107, 2, "Engine", "Coolant level normal"}})){}

    void updateseverity(const int diagId,const int severity)
    {
        // std::unique_lock<std::mutex> lock(m_mutex);
        auto expectedsnapshot = m_snapshot.load();
        while(true)
        {
            auto newsnapshot = *expectedsnapshot;

            auto it = std::ranges::find(newsnapshot,diagId,&Diagnostic::id);
            if(it != newsnapshot.end())
            {
                it->severity = severity;
            }
            std::shared_ptr<const std::vector<Diagnostic>> newsnapshot_p = std::make_shared<const std::vector<Diagnostic>>(newsnapshot);
            if(m_snapshot.compare_exchange_weak(expectedsnapshot,newsnapshot_p))
            {
                break;
            }
            // m_snapshot.store();

        }
    }

    // std::vector<Diagnostic> getDiagnostics()
    // {
    //     std::shared_lock<std::shared_mutex> lock(m_mutex);
    //     m_active_readers.fetch_add(1);
    //     auto result = m_diagnostics;
    //     std::cout<<" m_active_readers "<<m_active_readers<<std::endl;
    //     m_active_readers.fetch_sub(1);
    //     return result;
    // }

    std::shared_ptr<const std::vector<Diagnostic>> getsnapShot() const{
        return m_snapshot.load();
    }

private:
    mutable std::mutex m_mutex;
    std::atomic<std::shared_ptr<const std::vector<Diagnostic>>> m_snapshot;
};


int main()
{

    DiagnosticRepository repo;
    std::vector<std::jthread> reader;

    for(int i = 0;i<5;i++)
    {
        reader.emplace_back([&repo](std::stop_token token){
            while(!token.stop_requested())
            {
                std::shared_ptr<const std::vector<Diagnostic>> result = repo.getsnapShot();
                auto it = std::ranges::max_element(*result,{},&Diagnostic::severity);
                if(it != result->end())
                {
                    // std::cout<<"thread id "<<std::this_thread::get_id()<<" diag id "<<it->id<<" severity "<<it->severity<<std::endl;
                }
                auto id_101 = std::ranges::find(*result,101,&Diagnostic::id);
                if(id_101 != result->end())
                {
                    std::cout<<"thread id "<<std::this_thread::get_id()<<" diag id "<<id_101->id<<" severity "<<id_101->severity<<std::endl;
                }
                auto id_102 = std::ranges::find(*result,102,&Diagnostic::id);
                if(id_102 != result->end())
                {
                    std::cout<<"thread id "<<std::this_thread::get_id()<<" diag id "<<id_102->id<<" severity "<<id_102->severity<<std::endl;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(200));

            }
        });
    }

    std::jthread writer([&repo](std::stop_token token){
        auto random_genrator = [](int min,int max)->int{
            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dist(min,max);
            return dist(gen);
        };
        while(!token.stop_requested())
        {
            // int did = random_genrator(101,107);
            // int severity = random_genrator(0,10);
            repo.updateseverity(101,8);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    });
    std::jthread writer2([&repo](std::stop_token token){
        while(!token.stop_requested())
        {
            // int did = random_genrator(101,107);
            // int severity = random_genrator(0,10);
            repo.updateseverity(102,9);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    });

    std::this_thread::sleep_for(std::chrono::seconds(5));
    for(auto& th : reader)
    {
        th.request_stop();
    }
    writer.request_stop();
    writer2.request_stop();
}