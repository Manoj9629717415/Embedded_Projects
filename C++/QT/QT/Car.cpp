#include <iostream>
#include <QObject>



class Car : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int speed 
               READ speed
               WRITE setSpeed
               NOTIFY speedChanged);

public:

    explicit Car(QObject* parent=nullptr):QObject(parent)
    {
        
    }

    void setSpeed(int speed)
    {
        if(m_speed == speed)
        {
            return;
        }

        m_speed = speed;

        emit speedChanged(m_speed);

    }

    int speed() const{
        return m_speed;
    }

    signals:
        void speedChanged(int speed);

private:
    int m_speed{0};

};