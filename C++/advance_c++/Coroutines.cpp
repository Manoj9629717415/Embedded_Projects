#include <coroutine>
#include <iostream>
#include <thread>
#include <chrono>
#include <future>

template<typename T>
struct Task{

    struct promise_type
    {
        std::promise<T> result_promise;
        Task get_return_object()
        {
            return Task{std::coroutine_handle<promise_type>::from_promise(*this),std::move(result_promise.get_future())};
        }

        std::suspend_never initial_suspend(){
            return {};
        }

        std::suspend_always final_suspend() noexcept{
            return {};
        }

        void return_value(T v){
           result_promise.set_value(v);
        }

        void unhandled_exception()
        {
            result_promise.set_exception(std::current_exception());
        }
        
    };

    Task(std::coroutine_handle<promise_type> h,std::future<T> f):handle(h),result(std::move(f)){
    }

  T get(){
        return result.get();
    }

    std::coroutine_handle<promise_type> handle;
    std::future<T> result;

     ~Task(){
        if(handle)
        {
            handle.destroy();
        }
     }
};

struct DiagnosticAwaiter{

    int diagnostic_id;
    int response = 0;

    DiagnosticAwaiter(int id):diagnostic_id(id){}

    bool await_ready()
    {
        return false;
    }

    bool await_suspend(std::coroutine_handle<> handle)
    {
        std::thread worker([handle,this](){
            std::cout<<" request sent for ID "<<diagnostic_id<<"\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));
            response = 42;
            handle.resume();
        });
        worker.detach();
        return true;
    }

    int await_resume(){

        return response;
    }

};

Task<int> diagnostics()
{
    std::cout<<"Before Request \n ";
    int result = co_await DiagnosticAwaiter{101};
    std::cout<<" Response "<<result<<"\n";
    throw std::runtime_error("Diagnostic request failed");
    co_return result;
}

int main()
{
    auto f = diagnostics();
    std::cout<<"Main continuzues \n";
    try{
        auto result = f.get();
        std::cout<<"did result "<<result<<"\n";
    }
    catch(const std::exception& e)
    {
        std::cerr<<e.what()<<"\n";
    }
    
}