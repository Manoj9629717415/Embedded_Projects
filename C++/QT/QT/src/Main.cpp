#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QUrl>
#include <QThread>
#include <QObject>

#include "Vehicle.h"
#include "Canworker.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    Vehicle vehicle;

    engine.rootContext()->setContextProperty("vehicle", &vehicle);

    // engine.loadFromModule("VehicleHMI", "Main");
    engine.load(QUrl(QStringLiteral("qrc:/VehicleHMI/qml/Main.qml")));

    if (engine.rootObjects().isEmpty())
    {
        return -1;
    }

    CanWorker worker;
    QThread thread;
    worker.moveToThread(&thread);

    


    QObject::connect(&worker,&CanWorker::speedReceived,&vehicle,&Vehicle::setSpeed);
    QObject::connect(&worker,&CanWorker::batteryLevelRecevied,&vehicle,&Vehicle::setBatteryLevel);
    QObject::connect(&thread,&QThread::started,&worker,&CanWorker::startSimulation);
    thread.start();



    int result = app.exec();

    thread.quit();
    thread.wait();

    return result;


}