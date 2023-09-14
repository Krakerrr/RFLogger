#include "dialog.h"

#include <QApplication>
#include "rfreader.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();

    RFreader rf;
    rf.openSerialPort();

    w.updateGui();

    return a.exec();
}
