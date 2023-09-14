#ifndef RFREADER_H
#define RFREADER_H

#include <QObject>
#include <QSerialPort>
#include <QDebug>
#include <stdint.h>

class RFreader : public QObject
{
    Q_OBJECT

private:
    QSerialPort *m_serialPort;
    const QString m_portName = "COM3";
    bool m_writeToFile = false;
    uint8_t m_RFdata[120] = {0};

    void parseRFData();

public:
    bool m_dataready = false;
    bool m_connectionstatus = false;

public:
    // construct decostruct
    explicit RFreader(QObject *parent = nullptr);
    ~RFreader();

signals:
    void dataready();

public slots:
    bool openSerialPort();
    void closeSerialPort();
    void readData();
    void handleError(QSerialPort::SerialPortError error);
};

#endif // RFREADER_H
