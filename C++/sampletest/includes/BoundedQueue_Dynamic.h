#ifndef BOUNDEDQUEUE_H
#define BOUNDEDQUEUE_H
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>



template<typename T>
class BoundedQueue_Dynamic{
public:
    explicit BoundedQueue_Dynamic(std::size_t capacity);

    void push(const T& data);

    void pop(T& data);

    void shutdown();


private: 
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_not_empty_slot;
    std::condition_variable m_not_full_slot;
    std::size_t m_capacity{0};
    std::atomic<bool> m_shutdown{false};
};

#include "BoundedQueue_Dynamic.tpp"

#endif