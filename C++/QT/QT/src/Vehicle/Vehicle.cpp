#include "Vehicle.h"
#include <QTimer>

Vehicle::Vehicle(QObject* parent):QObject(parent){

    // connect(&m_timer,&QTimer::timeout,[this](){
    //     int speed = m_speed;
    //     if(speed <= 200)
    //     {
    //         speed += 5;
    //     }
    //     else
    //     {
    //         speed = 0;
    //     }
    //     setSpeed(speed);
    // });
    // m_timer.start(100);

}

void Vehicle::setSpeed(int speed)
{
    if (m_speed == speed)
    {
        return ;
    }

    m_speed = speed;

    emit speedChanged();
}

void Vehicle::increaseSpeed()
{
    setSpeed(m_speed+10);
    // m_timer.start(100);
    
}

void Vehicle::decreaseSpeed()
{
    setSpeed(m_speed-10);
}

void Vehicle::openSettings()
{
    
}

int Vehicle::speed() const{
    return m_speed;
}

int Vehicle::batteryLevel() const
{
    return m_batteryLevel;
}

void Vehicle::setBatteryLevel(int batteryLevel)
{
    if(m_batteryLevel == batteryLevel)
        return ;

    m_batteryLevel = batteryLevel;
    emit batteryLevelChanged();
}

int Vehicle::temperature() const{
    return m_temperature;
}

void Vehicle::setTemperature(int temperature)
{
    if(m_temperature == temperature)
    {
        return;
    }

    m_temperature = temperature;

    emit temperatureChanged();
}