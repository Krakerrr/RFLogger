#include "serialcomm.h"

SerialComm::SerialComm(QObject *parent)
    : QObject{parent}
{
    qInfo() << this << "Constructed" << QThread::currentThread();

    // inits
    serialBuffer = "";
    parsed_data = "";
    pSerialPort = new QSerialPort(this);
    pSerialPort->setPortName("");
    pSerialPort->setBaudRate(QSerialPort::Baud115200);
    pSerialPort->setDataBits(QSerialPort::Data8);
    pSerialPort->setParity(QSerialPort::NoParity);
    pSerialPort->setStopBits(QSerialPort::OneStop);


    // slots connection
    connect(pSerialPort, &QSerialPort::readyRead, this, &SerialComm::getSerialData);
    connect(pSerialPort, &QSerialPort::errorOccurred, this, &SerialComm::handleError);
}

SerialComm::~SerialComm()
{
    closeSerialPort();
    qInfo() << this << "Deconstructed" << QThread::currentThread();

    // delete m_serialPort
    pSerialPort = nullptr;
    delete pSerialPort;

    // close log file
    openDataFile("",false); // close file
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

    if( fFileOpenStatus )
    {
        writeDataToFile(serialData);
    }
}

void SerialComm::getSerialDataTherad()
{
    qInfo() << "Running in" << QThread::currentThread();
    for (int i = 0; i < 10; ++i) {
        qInfo() << "Doing Stuff in" << QThread::currentThread();
        QThread::currentThread()->msleep(500);
    }
}

bool SerialComm::openDataFile(QString filename, bool Write)
{
    if( Write && !fFileOpenStatus)
    {
        // open file
        outfile.setFileName(filename + QString(".txt"));
        if( ! outfile.open(QIODevice::ReadWrite) )
        {
            qCritical() << "Could not open file!";
            qCritical() << outfile.errorString();
            return false;
        }else
        {
            fFileOpenStatus = true;
            return true;
        }
    }
    else if (!Write && fFileOpenStatus) {
        // close file
        fFileOpenStatus = false;
        outfile.flush();
        outfile.close();
        return true;
    }
    else
        return false;
}


bool SerialComm::openSerialPort(QString ComPort)
{
    qInfo() << "Recieved port name" << ComPort;
    pSerialPort->setPortName(ComPort);
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
        emit ConnectionError(pSerialPort->errorString());
    }
}
