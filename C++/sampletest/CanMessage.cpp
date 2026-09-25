#include <iostream>
#include <array>
#include <cstdint>


struct CanMessage
{
    uint32_t id;
    std::array<uint8_t, 8> data;
};

struct Entry{
    bool occupied{false};
    CanMessage msg;
}

class CanMessageStore
{
public:
  static constexpr std::size_t MAX_MESSAGES = 32;
    bool add(const CanMessage& msg){
        for(std::size_t i = 0;i<m_size;i++)
        {
            if(m_canmessages[i].id== msg.id)
            {
                m_canmessages[i].id = msg.id;
                m_canmessages[i].data = msg.data;
                return true;
            }
        }
        if(m_size == MAX_MESSAGES)
        {
            return false;
        }

        m_canmessages[m_size] = msg;
        m_size++;
        return true;

    }
    bool get(uint32_t id, CanMessage& msg)
    {
        for (std::size_t i = 0; i < m_size; ++i)
        {
            if (m_canmessages[i].id == id)
            {
                msg = m_canmessages[i];
                return true;
            }
        }
        return false;

    }

private:
    std::array<CanMessage,MAX_MESSAGES> m_canmessages{};
    std::size_t m_size{0};
};

class CanMessageStore_Hashtable
{
public:
  static constexpr std::size_t MAX_MESSAGES = 32;
    bool add(const CanMessage& msg){

        std::size_t index = hash(msg.id);

        for(std::size_t i = 0; i< MAX_MESSAGES;i++)
        {
            if(m_table[index].occupied == false)
            {
                m_table[index].occupied = true;
                m_table[index].msg = msg;
                return true;
            }

            if(m_table[index].msg.id == msg.id)
            {
                m_table[index].msg = msg;
                return true;
            }

            index = (index+1) % MAX_MESSAGES;
        }
        return false;

    }
    bool get(uint32_t id, CanMessage& msg)
    {
        std::size_t index = hash(id);

        for(std::size_t i = 0;i<MAX_MESSAGES;i++)
        {
            if(m_table[index].occupied==true && m_table[index].msg.id == id)
            {
                msg.data = m_table[index].msg.data;
                msg.id = m_table[index].msg.id;
                return true;
            }
            index = (index +1) % MAX_MESSAGES;

        }

        return false;

    }

    std::size_t hash(uint32_t id)
    {
        return id % MAX_MESSAGES;
    }


private:
    std::array<Entry,MAX_MESSAGES> m_table{};
    std::size_t m_size{0};
};