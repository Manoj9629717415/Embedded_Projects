#include <Canworker.h>


CanWorker::CanWorker(QObject* parent):QObject(parent){
    m_timer = new QTimer(this);
    m_batterytimer = new QTimer(this);
    connect(m_timer,&QTimer::timeout,[this](){
        if(m_speed < 200)
        {
            m_speed += 5;
        }
        else{
            m_speed = 0;
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
    m_batterytimer->start(3000);
}
