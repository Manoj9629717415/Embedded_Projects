#include <Canworker.h>
#include <nlohmann/json.hpp>


CanWorker::CanWorker(QObject* parent):QObject(parent){
    m_timer = new QTimer(this);
    m_batterytimer = new QTimer(this);
    connect(m_timer,&QTimer::timeout,[this](){
        nlohmann::json jsonObj = nlohmann::json::parse(m_jsonString);
        if(m_speed < 200)
        {
            m_speed = jsonObj["speed"].get<int>();
            jsonObj["speed"] = m_speed + 10;
            m_jsonString = jsonObj.dump();
        }
        else{
            m_speed = 0;
            jsonObj["speed"] = 0;
            m_jsonString = jsonObj.dump();
        }
        emit speedReceived(m_speed);
    });
    connect(m_batterytimer,&QTimer::timeout,[this](){
        if(m_batteryLevel < 100)
        {
            m_batteryLevel += 2;
        }
        else{
            m_batteryLevel =  0;
        }
        emit batteryLevelRecevied(m_batteryLevel);
    });
}


void CanWorker::startSimulation()
{
    
    m_timer->start(1000);
    m_batterytimer->start(1000);
}
