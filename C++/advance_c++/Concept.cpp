#include <iostream>
#include <concepts>
#include <future>
#include <chrono>

struct Car
{
    bool start()
    {
        std::cout << "Car started\n";
        return true;
    }
};

struct Engine
{
    bool start()
    {
        std::cout << "Engine started\n";
        return true;
    }
};

struct BadComponent{
    std::future<bool> start()
    {
        std::promise<bool> p;
        std::future<bool> f = p.get_future();
        std::thread worker([](std::promise<bool> pr){
            std::this_thread::sleep_for(std::chrono::seconds(2));
            pr.set_value(true);
        },std::move(p));
        worker.detach();
        return f;
    }
};
struct Wheel
{
    void rotate()
    {
        std::cout << "Wheel rotating\n";
    }
};

template<typename T>
concept Startable = requires(T obj)
{
    {obj.start()} -> std::same_as<bool>;
};

template<typename T>
concept Startable_Future = requires(T obj)
{
    {obj.start()} -> std::same_as<std::future<bool>>;
};

template<typename T>
requires Startable <T> || Startable_Future<T>
auto StartComponent(T& obj)
{
    return obj.start();
}

void printresult(auto result)
{
    if constexpr(std::same_as<decltype(result),bool>)
    {
        std::cout<< "result "<<result<<std::endl;
    }
    else if constexpr(std::same_as<decltype(result),std::future<bool>>)
    {
        auto re = result.get();
         std::cout<< "result "<<re<<std::endl;
    }

}

struct Sensor
{
    int read(){
        return 10;
    }
};

struct AsyncSensor
{
    std::future<int> read(){
        std::promise<int> p;
        std::future<int> f = p.get_future();
        std::thread t([](std::promise<int> pr){
            std::this_thread::sleep_for(std::chrono::seconds(2));
            pr.set_value(100);
        },std::move(p));
        t.detach();
        return std::move(f);
    }
};

template<typename T>
concept syncSensorReader = requires(T obj)
{
    {obj.read()}->std::same_as<int>;
};

template<typename T>
concept asyncSensorReader = requires(T obj)
{
    {obj.read()}->std::same_as<std::future<int>>;
};

template<typename T>
requires syncSensorReader<T> || asyncSensorReader<T>
void readSensor(T& sensor)
{
    auto result = sensor.read();
    if constexpr(syncSensorReader<T>)
    {
        std::cout<<" sensor value"<<result<<std::endl;
    }
    else if constexpr (asyncSensorReader<T>)
    {
        auto f = result.get();
        std::cout<<" sensor value"<<f<<std::endl;
    }
}

int main()
{
    Car C;
    printresult(StartComponent(C));

    Engine E;
    printresult(StartComponent(E));

    // Wheel W;
    // StartComponent(W);

    BadComponent B;
    printresult(StartComponent(B));

    Sensor S;
    readSensor(S);

    AsyncSensor AS;
    readSensor(AS);

    return 0;
}