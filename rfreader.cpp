#include "rfreader.h"

RFreader::RFreader(QObject *parent)
    : QObject{parent}
{

    m_serialPort = new QSerialPort(this);
    connect(m_serialPort, &QSerialPort::readyRead, this, &RFreader::readData);
    connect(m_serialPort, &QSerialPort::errorOccurred, this, &RFreader::handleError);
}

RFreader::~RFreader()
{
    // delete m_serialPort
    m_serialPort = nullptr;
    delete m_serialPort;

    closeSerialPort();
}

bool RFreader::openSerialPort()
{
    m_serialPort->setPortName(m_portName);
    m_serialPort->setBaudRate(QSerialPort::Baud115200);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    if (m_serialPort->open(QIODevice::ReadWrite)) {
        qInfo()<< "Opened serial port" <<m_serialPort->isOpen();
        return true;
    } else {
        qInfo()<< "Error serial port" <<m_serialPort->errorString();
        return false;
    }
}

void RFreader::closeSerialPort()
{
    if (m_serialPort->isOpen())
        qInfo()<< "Closed serial port" <<m_serialPort->isOpen();
    m_serialPort->close();
}

void RFreader::readData()
{
    qInfo() << "Data Ready: \n";
    const QByteArray data = m_serialPort->readAll();
    qInfo() << data;
}

void RFreader::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        qInfo() << "Critical Error" << m_serialPort->errorString();
        closeSerialPort();
    }
}

void RFreader::parseRFData()
{

}

