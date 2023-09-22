

#include <QApplication>
#include <QThread>
#include <QDebug>

#include "serialcomm.h"
#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QThread::currentThread()->setObjectName("main thread");

    SerialComm serial;
    serial.setObjectName("Rf reader");

    // app window
    Dialog w;
    w.setObjectName("Window");

    QObject::connect(&w, &Dialog::Connect, &serial, &SerialComm::openSerialPort);
    QObject::connect(&w, &Dialog::Disconnect, &serial, &SerialComm::closeSerialPort);
    QObject::connect(&w, &Dialog::WriteToText, &serial, &SerialComm::openDataFile);



    w.show();
    return a.exec();
}
