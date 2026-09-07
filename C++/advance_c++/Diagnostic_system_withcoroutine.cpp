#include <iostream>
#include <thread>
#include <coroutine>
#include <future>
#include <vector>
#include <functional>
#include <chrono>
#include <utility>
#include <atomic>

struct OperationState
{
    std::atomic<bool> cancelled{false};
    std::atomic<int> response{0};
    std::atomic<bool> completed{false};
};

template<typename T>
struct Task{

    struct promise_type;

    Task(std::coroutine_handle<promise_type> handle,std::future<T> f):m_handle(handle),m_result_f(std::move(f)){}

    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;

    Task(Task&& other) noexcept
    {
        m_handle = std::exchange(other.m_handle,{});
        m_result_f = std::move(other.m_result_f);
    }

    Task& operator=(Task&& other) noexcept
    {
        if (this != &other)
        {
            if (m_handle)
                m_handle.destroy();

            m_handle = std::exchange(other.m_handle, {});
            m_result_f = std::move(other.m_result_f);
        }
    }

    void cancel(){
        m_handle.promise().m_stop_source.request_stop();
    }

    std::stop_token get_stop_token() const
    {
        return m_handle.promise().m_stop_source.get_token();
    }

    struct promise_type{
        Task get_return_object()
        {
            return Task{std::coroutine_handle<promise_type>::from_promise(*this),std::move(m_promise.get_future())};
        }

        std::suspend_never initial_suspend() { return {};}

        std::suspend_always final_suspend() noexcept { return {};}

        void return_value(T v)
        {
            m_promise.set_value(std::move(v));
        }

        void unhandled_exception()
        {
            m_promise.set_exception(std::current_exception());
        }

        std::promise<T> m_promise;
        std::stop_source m_stop_source;
    };

    T get()
    {
        return m_result_f.get();
    }

    std::coroutine_handle<promise_type> m_handle{};
    std::future<T> m_result_f;
    ~Task()
    {
        if (m_handle)
        {
            m_handle.destroy();
            m_handle = {};
        }
    }
};



class DiagnocticsECU{
public:
    void sendRequest(int did,std::function<void(int,std::exception_ptr)> callback)
    {
        std::thread th([did,t=std::move(callback)](){
             std::cout << "ECU: request sent for " << did << '\n';

            std::this_thread::sleep_for(
                std::chrono::seconds(2));

            std::cout << "ECU: response received for did "<<did<<"\n";

            try{
                t(42,nullptr);
            }
            catch(...)
            {
                t(0,std::current_exception());
            }

        });
        th.detach();
    }
};

struct DiagonisticsAwaiter{

    DiagnocticsECU& m_diag_ecu;
    std::shared_ptr<OperationState> m_state;
    int m_did{0};
    std::stop_token m_token;

    DiagonisticsAwaiter(DiagnocticsECU& ecu,int did,std::shared_ptr<OperationState> state,std::stop_token token):
                                              m_diag_ecu(ecu),
                                              m_did(did),
                                              m_state(state),
                                              m_token(token){}


    bool await_ready()
    {
        return false;
    }

    bool await_suspend(std::coroutine_handle<> handle)
    {
        auto state = m_state;
        auto token = m_token;
        auto callback = [state,token,handle](int resonse,std::exception_ptr err){
            if(err)
            {
                throw std::runtime_error(" exception occured");
                return;
            }
            state->response = resonse;
            std::cout<<" response received \n";

            bool expected = false;

            if(!state->completed.compare_exchange_strong(expected,true))
            {
                std::cout<<" already completed ro cancelled \n";
                return;
            }

            if(token.stop_requested() || state->cancelled.load())
            {
                return;
            }

            // if(!m_state->cancelled.load())
            // {
                std::cout<<" response received but stop not requested \n";
                handle.resume();
            // }
        };
        m_diag_ecu.sendRequest(m_did,callback);
        return true;
    }

    int await_resume(){
        return m_state->response;
    }

};

Task<int> readDiagnostics(DiagnocticsECU& ecu,int did)
{
    std::cout<<" before request \n";

    auto& promise = std::coroutine_handle<Task<int>::promise_type>::from_promise(co_await std::suspend_never{}).promise();
    
    std::shared_ptr<OperationState> state = std::make_shared<OperationState>();

    auto response = co_await DiagonisticsAwaiter{ecu,did,state,promise.m_stop_source.get_token()};

    std::cout << "Response for did " <<did<<" received: " << response << '\n';

    if(promise.m_stop_source.stop_requested())
    {
        std::cout << "Stop requested Diag operation cancelled" << '\n';
        state->cancelled.store(true);

         std::cout<< "Coroutine: operation cancelled\n";

        co_return -1;
    }
    
    co_return response;

}


int main()
{
    DiagnocticsECU ecu;

    auto task = readDiagnostics(ecu, 101);
    // auto task1 = std::move(task);

    std::cout << "Main continues\n";

    task.cancel();

    try{

        int result = task.get();

        std::cout << "Result = " << result << '\n';
    }
    catch(const std::exception& e)
    {
        std::cerr<<e.what()<<"\n";
    }
    // try{

    //     int result1 = task1.get();

    //     std::cout << "Result = " << result1 << '\n';
    // }
    // catch(const std::exception& e)
    // {
    //     std::cerr<<e.what()<<"\n";
    // }

}