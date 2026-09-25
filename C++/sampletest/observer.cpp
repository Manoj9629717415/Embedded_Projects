#include <vector>
#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>

struct CanMessage
{
    uint32_t id;
    std::array<uint8_t, 8> data;
};


class IObserver {
public:
    virtual ~IObserver() = default;

    virtual void onMessage(const CanMessage& msg) = 0;

};


class DiagReceiver : public IObserver{
public:
    void onMessage(const CanMessage& msg)
    {
        std::cout<<"DiagReceiver id "<<msg.id<<"\n";

    }
};


class LoggerReceiver : public IObserver{
public:
    void onMessage(const CanMessage& msg)
    {
        std::cout<<"LoggerReceiver id "<<msg.id<<"\n";
    }
};

class CANMessageSubject {
public:
    void subscribe(IObserver& obj)
    {
        m_observers.push_back(&obj);
    }

    void unsubcribe(IObserver& obj)
    {
        m_observers.erase(std::remove(m_observers.begin(),m_observers.end(),&obj),m_observers.end());

    }

    void notify(const CanMessage& msg)
    {
        for(auto* obersver : m_observers )
        {
            if(obersver != nullptr)
            {
                obersver->onMessage(msg);
            }
        }
    }

private:
    std::vector<IObserver*> m_observers;

};

int main()
{
    CANMessageSubject subject;
    DiagReceiver diagreceiver;
    LoggerReceiver logrecevier;

    subject.subscribe(diagreceiver);
    subject.subscribe(logrecevier);

    CanMessage msg{1001,{10,10,10,10,10,10,10,10}};
    subject.notify(msg);

    std::vector<int> v{5, 2, 8, 1, 3};

    std::sort(v.begin(),v.end(),std::greater<int>{});

    std::vector<int> v1{1, 2, 3, 4};

    std::transform(
        v.begin(),
        v.end(),
        v.begin(),
        [](int x)
        {
            return x * 2;
        });



}
