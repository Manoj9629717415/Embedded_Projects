#ifndef CAN_WORKER_H
#define CAN_WORKER_H

#include <QTimer>
#include <QObject>


class CanWorker : public QObject{
    Q_OBJECT

public:

    explicit CanWorker(QObject* parent = nullptr);

public slots:
    void startSimulation();

signals:
    void speedReceived(int speed);
    void batteryLevelRecevied(int batteryLevel);

private:
    int m_speed{0};
    int m_batteryLevel{0};
    QTimer* m_timer;
    QTimer* m_batterytimer;

};






#endif