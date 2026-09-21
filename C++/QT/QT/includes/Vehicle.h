#ifndef VEHICLE_H
#define VEHICLE_H

#include <QObject>
#include <QTimer>

class Vehicle : public QObject{

    Q_OBJECT

    Q_PROPERTY(int speed READ speed WRITE setSpeed NOTIFY speedChanged)
    Q_PROPERTY(int batteryLevel READ batteryLevel WRITE setBatteryLevel NOTIFY batteryLevelChanged)
    Q_PROPERTY(int temperature READ temperature WRITE setTemperature NOTIFY temperatureChanged)



public:

    explicit Vehicle(QObject* parent = nullptr );

    Q_INVOKABLE
    void increaseSpeed();

    Q_INVOKABLE
    void decreaseSpeed();

    Q_INVOKABLE
    void openSettings();

    int speed() const;
    void setSpeed(int speed);

    int batteryLevel() const;
    void setBatteryLevel(int batterylevel);

    int temperature() const;
    void setTemperature(int temperature);


signals:
    void speedChanged();
    void batteryLevelChanged();
    void temperatureChanged();

private:
    int m_speed = 0;
    int m_batteryLevel = 100;
    int m_temperature = 20;
    QTimer m_timer;



};

#endif