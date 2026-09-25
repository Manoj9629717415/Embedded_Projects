#include <cstdint>
#include <iostream>
#include <utility>

class Buffer
{
public:
    Buffer(std::size_t size)
        : m_size(size),
          m_data(new uint8_t[size])
    {
    }

    Buffer(const Buffer& other)
    {
        m_size = other.m_size;
        m_data = new uint8_t[m_size];
        std::copy(other.m_data,other.m_data+other.m_size,this->m_data);
    }

    Buffer& operator =(const Buffer& other){
        if(this != &other)
        {
            delete[] m_data;
            this->m_size = other.m_size;
            this->m_data = new uint8_t[m_size];
            std::copy(other.m_data,other.m_data+other.m_size,this->m_data);
        }
        return *this;
    }

    Buffer( Buffer&& other) noexcept
    {
        m_size = std::exchange(other.m_size,0);
        m_data = std::exchange(other.m_data,nullptr);

    }

    Buffer& operator =( Buffer&& other) noexcept{
        if(this != &other)
        {
            delete[] m_data;
            this->m_size = std::exchange(other.m_size,0);
            this->m_data = std::exchange(other.m_data,nullptr);
        }
        return *this;
    }

    ~Buffer()
    {
        delete[] m_data;
    }

private:
    std::size_t m_size;
    uint8_t* m_data;
};