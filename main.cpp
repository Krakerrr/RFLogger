

#include <QApplication>
#include <QThread>
#include <QDebug>

#include "serialcomm.h"
#include "dialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SerialComm rf;
    rf.setObjectName("Rf reader");


    // app window
    Dialog w;
    w.setObjectName("Window");

    QObject::connect(&w, &Dialog::Connect, &rf, &SerialComm::openSerialPort);
    QObject::connect(&w, &Dialog::Disconnect, &rf, &SerialComm::closeSerialPort);

    w.show();
    return a.exec();
}
