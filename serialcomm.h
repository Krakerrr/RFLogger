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


class SerialComm : public QObject
{
    Q_OBJECT
public:
    explicit SerialComm(QObject *parent = nullptr);
    ~SerialComm();
    bool fDataReady = false;
    bool fConnectionStatus = false;
    bool fFileOpenStatus = false;

private:
    QSerialPort* pSerialPort;
    const QString portName = "COM3";
    QByteArray serialData;
    QByteArray serialBuffer;
    QString parsed_data;
    QElapsedTimer timer;
    QFile outfile;
    void writeDataToFile(const QByteArray &data);
    bool closeDataFile(void);

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
