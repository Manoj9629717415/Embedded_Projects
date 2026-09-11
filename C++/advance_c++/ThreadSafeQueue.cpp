#include <iostream>
#include <queue>
#include <condition_variable>
#include <map>
#include <thread>

template<typename T>
class ThreadSafeQueue {
public:
    void push(T element)
    {
        {
            std::lock_guard<std::mutex> lock(data_mutext);

            data.emplace(std::move(element));
           
        }
        cv.notify_one();
    }

    bool pop(T& value){
        std::unique_lock<std::mutex> lock(data_mutext);

        cv.wait(lock,[this]{
            return !data.empty() || flag;
        });

        if(data.empty() && flag)
        {
            return false;
        }

        value = std::move(data.front());
        data.pop();
        return true;
    }

    void stop()
    {
        {
            std::lock_guard<std::mutex> lock(data_mutext);
            flag = true;
        }
        cv.notify_one();
    }


private:
std::queue<T> data{};
std::mutex data_mutext;
std::condition_variable cv;
bool flag{false};
};

struct SpeedData
{
    uint8_t speed;
    uint16_t rpm;
    SpeedData(uint8_t sp,uint16_t r):speed(sp),rpm(r){}
    SpeedData(){}
};


int main()
{
   
    ThreadSafeQueue<SpeedData> datacontainer;
    std::thread produce([&datacontainer]{
        for(int i=0;i<10;i++)
        {
            datacontainer.push(SpeedData(i*10,(i*1000+1000)));
        }
        datacontainer.stop();
    });

    std::thread consume([&datacontainer]{
        SpeedData data;
        while(datacontainer.pop(data))
        {
            std::cout<<"consumed data speed "<<data.speed<<" rpm "<<data.rpm<<std::endl;
        }
    });

    produce.join();
    consume.join();

    return 0;
}
