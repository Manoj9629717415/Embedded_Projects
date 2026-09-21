#include <QCoreApplication>
#include <QObject>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    qDebug() << "Qt is working!";
    qDebug() << "QObject:" << sizeof(QObject);

    return 0;
}