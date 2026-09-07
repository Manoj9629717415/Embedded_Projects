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

struct Diagnostic{
    int id;
    int severity;
    std::string component;
    std::string message;
};

template<typename T>
concept DiagnosticReadable = requires(T obj)
{
    {obj.read()} -> std::same_as<std::vector<Diagnostic>>;
};

// template<typename T>
// concept DiagnosticReadable = requires(T obj)
// {
//     obj.read();
// };

class DiagonsticsReader{
public:
    std::vector<Diagnostic> read() { return diagnostics;}
private:

};

class Threadpool{
public:
    explicit Threadpool(int num_of_worker){
        for(int i=0;i<num_of_worker;i++){
            m_worker.emplace_back([this](std::stop_token token){worker(token);});
        }
    }

    void worker(std::stop_token token)
    {
        while(true){
            std::unique_lock<std::mutex> lock(m_mutex);

            m_cv.wait(lock,token,[this](){
                return !m_tasks.empty();
            });

            if( token.stop_requested() && m_tasks.empty())
            {
                break;
            }

            auto task = std::move(m_tasks.front());
            m_tasks.pop();
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(1));
            task();
            m_processed_count.fetch_add(1);
        }

    }

    template<typename F,typename... Args>
    auto submit(F&& function,Args&&... arg){

        auto callable = std::bind(std::forward<F>(function),std::forward<Args>(arg)...);

        using ReturnType = std::invoke_result_t<F,Args...>;

        std::packaged_task<ReturnType()> task(std::move(callable));

        auto result = task.get_future();

        std::packaged_task<void()> wrapper([t=std::move(task)]() mutable 
        { t(); 
        });

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_tasks.push(std::move(wrapper));
        }
        m_cv.notify_one();
        return result;
    }

    ~Threadpool()
    {
        for(auto& th : m_worker)
        {
            th.request_stop();
        }
        std::cout<<"number of diag task "<<m_processed_count.load();
    }


private:
    std::vector<std::jthread> m_worker;
    std::mutex m_mutex;
    std::condition_variable_any m_cv;
    std::queue<std::packaged_task<void()>> m_tasks;
    std::atomic<int> m_processed_count{0};
};

class DiagnosticRepository{
public:
    void updateseverity(const int diagId,const int severity)
    {
        std::unique_lock<std::shared_mutex> lock(m_mutex);
        m_activeWriters.fetch_add(1);
        auto it = std::ranges::find(m_diagnostics,diagId,&Diagnostic::id);
        if(it != m_diagnostics.end())
        {
            it->severity = severity;
        }
        m_activeWriters.fetch_sub(1);
        std::cout<<" m_activeWriters "<<m_activeWriters<<std::endl;
    }

    std::vector<Diagnostic> getDiagnostics() const
    {
        std::shared_lock<std::shared_mutex> lock(m_mutex);
        m_active_readers.fetch_add(1);
        auto result = m_diagnostics;
        std::cout<<" m_active_readers "<<m_active_readers<<std::endl;
        m_active_readers.fetch_sub(1);
        return result;
    }
private:
    std::vector<Diagnostic> m_diagnostics{
        {101, 2, "Engine", "Oil temperature normal"},
        {102, 5, "Engine", "Engine overheating"},
        {103, 3, "Brake", "Brake pad wear"},
        {104, 4, "Transmission", "Transmission temperature high"},
        {105, 1, "Battery", "Battery voltage normal"},
        {106, 5, "Brake", "Brake pressure failure"},
        {107, 2, "Engine", "Coolant level normal"}
    };
    mutable std::shared_mutex m_mutex;
    std::atomic<int> m_active_readers{0};
    std::atomic<int> m_activeWriters{0};
};

template<DiagnosticReadable T>
auto processDiagnostics(T& reader)
{
    auto result = reader.read();
    auto ids = result
               | std::views::filter([](const Diagnostic& obj){ return obj.severity >= 4;  })
               | std::views::transform([](const Diagnostic& obj){ return obj.id;})
               | std::views::take(3);
            //    | std::ranges::to<std::vector>();
    
    // std::vector<int> result_vector(ids.begin(),ids.end());

    for(auto id : ids)
    {
        std::cout<<"id "<<id<<std::endl;
    }
    
               
    auto max_sev = std::ranges::max_element(result,{},&Diagnostic::severity);
    if(max_sev != result.end())
    {
    std::cout<<"Max severity id "<<max_sev->id<<" severity  "<<max_sev->severity<<" component "<<max_sev->component<<" message "<<max_sev->message<<std::endl;
    }

    bool is_all_severity_valid = std::ranges::all_of(result,[](const Diagnostic& obj){ return obj.severity <=5;});
    bool is_critical_diag_available = std::ranges::any_of(result,[](const Diagnostic& obj){ return obj.severity >= 4; });
    bool all_valid_diag = std::ranges::none_of(result,[](const Diagnostic& obj){ return obj.severity > 5;});
    std::cout<<"is_all_severity_valid "<<is_all_severity_valid<<std::endl;
    std::cout<<"is_critical_diag_available "<<is_critical_diag_available<<std::endl;
    std::cout<<"all_valid_diag "<<all_valid_diag<<std::endl;
    return result;
}

int num_of_critical_diag(const std::vector<Diagnostic>& diag)
{
    int num_of_critical = 0;
    if(!diag.empty()){
        num_of_critical= std::ranges::count_if(diag,[](const Diagnostic& d){return d.severity >= 4; });
    }
    return num_of_critical; 
}

std::optional<Diagnostic> highestseverityDiag(const std::vector<Diagnostic>& diag)
{
    auto it = std::ranges::max_element(diag,{},&Diagnostic::severity);
    if(it == diag.end())
    {
        return std::nullopt;
    }
    return *it;
}

std::vector<int> diag_severity_greather3(const std::vector<Diagnostic>& diag)
{
    //g++13 not suporting ranges::to
    auto result = diag
                    | std::views::filter([](const Diagnostic& d ){ return d.severity >= 3;})
                    | std::views::transform([](const Diagnostic& d ){ return d.id;});
                //   | std::ranges::to<std::vector>();
    std::vector<int> result_vector(result.begin(),result.end());
    return result_vector;

}


int main()
{
    DiagonsticsReader diag_reader;
    auto result = processDiagnostics(diag_reader);

    Threadpool pool(3);
    auto critical_diag_future = pool.submit(num_of_critical_diag,result);
    auto high_severity_future = pool.submit(highestseverityDiag,result);
    auto diagids_future = pool.submit(diag_severity_greather3,result);
    auto failure_future = pool.submit([]() -> int
    {
        throw std::runtime_error(
            "Diagnostic processing failed");

        return 0;
    });

    try
    {
        std::cout<<" num_of_critical_diag "<< critical_diag_future.get();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    try
    {
        auto diag = high_severity_future.get();
        if (diag)
        {
   
            std::cout<<" high_severity_future diag id "<< diag->id;
        }   
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    try
    {
        auto diags = diagids_future.get();
        for(auto& ids : diags )
        {
            std::cout<<" diag_severity_greather3 "<<ids;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    try
    {
        failure_future.get();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    DiagnosticRepository repo;
    std::vector<std::jthread> reader;

    for(int i = 0;i<5;i++)
    {
        reader.emplace_back([&repo](std::stop_token token){
            while(!token.stop_requested())
            {
                std::vector<Diagnostic> result = repo.getDiagnostics();
                auto it = std::ranges::max_element(result,{},&Diagnostic::severity);
                if(it != result.end())
                {
                    std::cout<<"thread id "<<std::this_thread::get_id()<<" diag id "<<it->id<<" severity "<<it->severity<<std::endl;
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
            int did = random_genrator(101,107);
            int severity = random_genrator(0,10);
            repo.updateseverity(did,severity);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    });

    std::this_thread::sleep_for(std::chrono::seconds(5));
    for(auto& th : reader)
    {
        th.request_stop();
    }
    writer.request_stop();

}


