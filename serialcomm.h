#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#include <QObject>
#include <QSerialPort>
#include <QThread>
#include <QDebug>
#include <QElapsedTimer>
#include <stdint.h>
#include <QFile>
#include <QTextStream>

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
    const QString portName = "COM4";
    QByteArray serialData;
    QByteArray serialBuffer;
    QString parsed_data;
    QElapsedTimer timer;
    bool fWriteFile = true;
    QFile outfile;
    void writeDataToFile(const QByteArray &data);

public slots:
    void getSerialData();
    bool openSerialPort();
    void closeSerialPort();
    void handleError(QSerialPort::SerialPortError error);

signals:
     void dataready();
};

#endif // SERIALCOMM_H
