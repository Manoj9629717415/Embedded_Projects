#include "BoundedQueue_Dynamic.h"

template<typename T>
BoundedQueue_Dynamic<T>::BoundedQueue_Dynamic(std::size_t capacity):m_capacity(capacity) {
}

template<typename T>
void BoundedQueue_Dynamic<T>::push(const T& data)
{
    while(true)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_not_full_slot.wait(lock,[this](){ return m_queue.size() < m_capacity || m_shutdown;});

        if(m_shutdown) break;

        m_queue.push(data);

        lock.unlock();

        m_not_empty_slot.notify_one();
    }

}

template<typename T>
void BoundedQueue_Dynamic<T>::pop( T& data)
{
    while(true)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_not_empty_slot.wait(lock,[this](){ return m_queue.size() > 0 || m_shutdown;});

        if(m_shutdown) break;

        data = m_queue.pop();

        lock.unlock();

        m_not_full_slot.notify_one();
    }

}

template<typename T>
void BoundedQueue_Dynamic<T>::shutdown()
{
    m_shutdown.store(true);
    m_not_full_slot.notify_one();
    m_not_empty_slot.notify_one();
}

// template<typename T>
// std::size_t BoundedQueue_Dynamic<T>::getsize(){
//     return m_queue.size();
// }

