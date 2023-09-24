#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include <QObject>
#include <QSerialPort>
#include <QThread>
#include <QElapsedTimer>
#include <stdint.h>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "Rfstruct.h"
#include "ringbuffer.h"


class SerialComm : public QObject
{
    Q_OBJECT


public:
    explicit SerialComm(QObject *parent = nullptr);
    ~SerialComm();
    bool fDataReady = false;
    bool fConnectionStatus = false;
    bool fFileOpenStatus = false;
    RingBuffer ringbuffer;
    struct RFData sRF;
    uint32_t recievedDataCounter = 0;
    uint32_t CRCnotOKCounter = 0;

private:
    QSerialPort* pSerialPort;
    QByteArray serialData;
    QElapsedTimer timer;
    QFile outfile;
    void writeDataToFile(const QByteArray &data);
    bool closeDataFile(void);
    void parseData(void);

public slots:
    void getSerialData();
    void getSerialDataTherad();
    bool openDataFile(QString filename, bool Write);
    bool openSerialPort(QString ComPort);
    void closeSerialPort();
    void handleError(QSerialPort::SerialPortError error);


signals:
    void dataready();
    void ConnectionError(QString errormsg);
};

#endif // SERIALCOMM_H
