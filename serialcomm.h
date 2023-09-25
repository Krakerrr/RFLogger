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

private:
    QSerialPort* pSerialPort;
    QByteArray serialData;
    QElapsedTimer timer;
    QFile outFileRaw;
    QFile outFileParsed;
    void writeDataToFile(const QByteArray &data);
    void writeDataToFile(const struct RFData& RF);
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
    void dataready(const struct RFData& RF);
    void ConnectionError(QString errormsg);
};

#endif // SERIALCOMM_H
