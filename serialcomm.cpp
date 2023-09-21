#include "serialcomm.h"

SerialComm::SerialComm(QObject *parent)
    : QObject{parent}
{
    qInfo() << this << "Constructed" << QThread::currentThread();

    // inits
    serialBuffer = "";
    parsed_data = "";
    pSerialPort = new QSerialPort(this);
    // slots connection
    connect(pSerialPort, &QSerialPort::readyRead, this, &SerialComm::getSerialData);
    connect(pSerialPort, &QSerialPort::errorOccurred, this, &SerialComm::handleError);


    // open file
    outfile.setFileName("out.txt");
    if( ! outfile.open(QIODevice::ReadWrite) )
    {
        qCritical() << "Could not open file!";
        qCritical() << outfile.errorString();
    }else
    {
        fFileOpenStatus = true;
    }
}

SerialComm::~SerialComm()
{
    closeSerialPort();
    qInfo() << this << "Deconstructed" << QThread::currentThread();

    // delete m_serialPort
    pSerialPort = nullptr;
    delete pSerialPort;

    outfile.close();
}

void SerialComm::writeDataToFile(const QByteArray &data)
{
    QTextStream out(&outfile);
    for (int i = 0; i < data.size(); ++i) {
        out << (quint8)data.at(i) << " ";
    }
}

void SerialComm::getSerialData()
{
    qInfo() << "Reading data" << QThread::currentThread();
    qInfo() << "Bytes available" << pSerialPort->bytesAvailable();
    serialData = pSerialPort->readAll();
    serialBuffer = serialBuffer + serialData.toHex(' ');

    qInfo() << "Time Elapsed:" << timer.nsecsElapsed() / 1e6;
    qInfo() << "Time Elapsed:" << timer.restart();


    if(fWriteFile && fFileOpenStatus)
    {
        writeDataToFile(serialData);
    }
}

bool SerialComm::openSerialPort()
{
    pSerialPort->setPortName(portName);
    pSerialPort->setBaudRate(QSerialPort::Baud115200);
    pSerialPort->setDataBits(QSerialPort::Data8);
    pSerialPort->setParity(QSerialPort::NoParity);
    pSerialPort->setStopBits(QSerialPort::OneStop);
    if (pSerialPort->open(QIODevice::ReadWrite)) {
        qInfo()<< "Opened serial port" <<pSerialPort->isOpen();
        fConnectionStatus = true;
        timer.start();
        return true;
    } else {
        qInfo()<< "Error serial port" <<pSerialPort->errorString();
        fConnectionStatus = false;
        return false;
    }
}

void SerialComm::closeSerialPort()
{
    if (pSerialPort->isOpen())
    {
        qInfo()<< "Closed serial port" <<pSerialPort->isOpen();
        pSerialPort->close();
        fConnectionStatus = false;
    }
}

void SerialComm::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        qInfo() << "Critical Error" << pSerialPort->errorString();
        closeSerialPort();
    }
}
