#include <iostream>
#include <array>
#include <cstdint>
#include <atomic>


constexpr std::size_t BUFFER_SIZE = 10;


struct DiagnosticMessage{
    uint8_t id;
    std::array<uint8_t,8> data;
};

class DiagnosticQueue{
public:
    bool push(const DiagnosticMessage& msg)
    {
        bool result = false;
        if(m_count == BUFFER_SIZE)
            return result;
            
        m_buffer[m_tail] = msg;

        m_tail = (m_tail + 1) % BUFFER_SIZE;
        ++m_count;

        result = true;

        return result;
    }

    bool pop(DiagnosticMessage& msg)
    {
        bool result = false;
        if(m_count == 0)
            return false;

        msg = m_diagmsg_buffer[m_head];

        head = (head +1) % BUFFER_SIZE;
        --m_count;

        return true;
    }

private:
    std::array<DiagnosticMessage,BUFFER_SIZE> m_diagmsg_buffer{};
    uint8_t m_head{0};
    uint8_t m_tail{0};
    uint8_t m_count{0};
    
};

template<typename T,std::size_t capacity>
class SpscQue{
public:
    bool push(const T& msg){
        const auto tail = m_tail.load(std::memory_order_relaxed);
        const auto head = m_head.load(std::memory_order_acquire);

        if(tail - head >= capacity)
            return false;

        m_buffer[tail % capacity] = msg;
        m_tail.store(tail + 1 ,std::memory_order_release);
        return true;
    }

    bool pop(T& msg){
        const auto head = m_head.load(std::memory_order_relaxed);
        const auto tail = m_tail.load(std::memory_order_acquire);

        if(head == tail)
            return false;

        msg = m_buffer[head % Capacity];

        m_head.store(head + 1, std::memory_order_release);

        return true;
    }

private:
    std::array<T,capacity> m_buffer;

    std::atomic<std::size_t> m_head{0};
    std::atomic<std::size_t> m_tail{0};


};

