#include <iostream>
#include <array>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>


template<typename T,std::size_t N>
class BoundedQueue{
public:
    bool push(const T& data)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if(m_count == N) return false;
        
        m_tail = m_tail % N;
        m_cache[m_tail] = data;
        m_count++;
        m_tail++;
        std::cout<<" elements inserted "<<data<<"\n";

        return true;
    }

    bool pop(T& data)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        if(m_count == 0) return false;

        m_head = m_head % N;
        data = m_cache[m_head];
        m_head++;
        m_count--;

        return true;
    }

private: 
    std::array<T,N> m_cache;
    std::size_t m_head{0};
    std::size_t m_tail{0};
    std::size_t m_count{0};
    std::mutex m_mutex;
};

template<typename T,std::size_t N>
class BoundedQueue_LockFree{
public:
    bool push(const T& data)
    {

        std::size_t tail = m_tail.load(std::memory_order_relaxed);
        std::size_t head = m_head.load(std::memory_order_acquire);

        if(tail - head >= N) return false;
        
        std::size_t index = tail % N;
        m_cache[index] = data;
        m_tail.store(tail +1,std::memory_order_release);
        std::cout<<" elements inserted "<<data<<"\n";
        return true;
    }

    bool pop(T& data)
    {
        std::size_t head = m_head.load(std::memory_order_relaxed);
        std::size_t tail = m_tail.load(std::memory_order_acquire);

        if(head == tail) return false;

        const std::size_t index = head % N;
        data = m_cache[index];
        m_head.store(head + 1,std::memory_order_release);

        return true;
    }

private: 
    std::array<T,N> m_cache;
    std::atomic<std::size_t> m_head{0};
    std::atomic<std::size_t> m_tail{0};
};




int main()
{
    // BoundedQueue<int,10> queue;
    
    // std::thread th1([&queue](){

    //     int fact = 3;
    //     for(int i = 0;i<20;i++)
    //     {
    //         if (!queue.push(i)) std::cout<<"insertion failure \n";
    //         if(i == fact )
    //         {
    //             std::this_thread::sleep_for(std::chrono::seconds(1));
    //             fact += 3;
    //         }
    //     }
    // });

    //  std::thread th2([&queue](){

    //     int fact = 2;
    //     int data = 0;
    //     for(int i = 0;i<20;i++)
    //     {
    //         if(!queue.pop(data)){
    //             std::cout<<"pop failure \n";
    //         }
    //         else{
    //             std::cout<<" elements poped "<<data<<"\n";
    //         }
    //         if(i == fact)
    //         {
    //             std::this_thread::sleep_for(std::chrono::seconds(1));
    //             fact += 2;
    //         }
    //     }
    // });

    // th1.join();
    // th2.join();
    BoundedQueue_LockFree<int,10> queue;
    
    std::thread th1([&queue](){

        int fact = 3;
        for(int i = 0;i<20;i++)
        {
            if (!queue.push(i)) std::cout<<"insertion failure \n";
            if(i == fact )
            {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                fact += 3;
            }
        }
    });

     std::thread th2([&queue](){

        int fact = 2;
        int data = 0;
        for(int i = 0;i<20;i++)
        {
            if(!queue.pop(data)){
                std::cout<<"pop failure \n";
            }
            else{
                std::cout<<" elements poped "<<data<<"\n";
            }
            if(i == fact)
            {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                fact += 2;
            }
        }
    });

    th1.join();
    th2.join();



}