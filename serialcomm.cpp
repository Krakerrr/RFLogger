#include "serialcomm.h"

SerialComm::SerialComm(QObject *parent)
    : QObject{parent}
{
    qInfo() << this << "Constructed" << QThread::currentThread();

    // inits
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
    QTextStream out(&outFileRaw);
    for (int i = 0; i < data.size(); ++i) {
        out << (quint8)data.at(i) << " ";
    }
}

void SerialComm::writeDataToFile(const RFData &RF)
{
    QTextStream out(&outFileParsed);
    out <<  sRF.FCC_Clock << " ";
    out <<  sRF.IMU_freq_status << " ";
    out <<  sRF.IMU_acc_x << " ";
    out <<  sRF.IMU_acc_y << " ";
    out <<  sRF.IMU_acc_z << " ";
    out <<  sRF.IMU_gyro_x << " ";
    out <<  sRF.IMU_gyro_y << " ";
    out <<  sRF.IMU_gyro_z << " ";
    out <<  sRF.IMU_temp << " ";
    out <<  sRF.CMD_throttle << " ";
    out <<  sRF.CMD_de << " ";
    out <<  sRF.CMD_da << " ";
    out <<  sRF.CMD_dr << " ";
    out <<  "\n";
}

void SerialComm::parseData(void)
{
    while(ringbuffer.GetAvailabeData() > RFDATASIZE*2)
    {
        uint8_t header, length;
        length = ringbuffer.Recieve();
        header = ringbuffer.Recieve();
        if( length == RFLENGTH && header == RFHEADER)
        {
            uint16_t CRCcalc, CRCrecv;
            uint8_t rawData[RFPAYLOADSIZE] = {0};
            CRCcalc   = RFHEADER + RFLENGTH;
            for (int iter = 0; iter < RFPAYLOADSIZE; ++iter) {
                rawData[iter] =ringbuffer.Recieve();
                CRCcalc +=rawData[iter];
            }
            CRCrecv = ((uint16_t) ringbuffer.Recieve() ) | ((uint16_t) ringbuffer.Recieve() << 8);
            if(CRCcalc == CRCrecv)
            {
                memcpy(&sRF,rawData,RFPAYLOADSIZE);
                sRF.Counter.recievedPacketCounter++;
                qInfo() << "FCC CLOCK = " << sRF.FCC_Clock;
                qInfo()<< "------CRC OK----";
                writeDataToFile(sRF);
                emit dataready(sRF);
            }
            else
            {
                qWarning()<< "----CRC NOT OK----";
                ringbuffer.MoveReadIndex(-RFPAYLOADSIZE -2 - 1);
                sRF.Counter.CRCnotOKCounter++;
            }
        }else{
            ringbuffer.MoveReadIndex(-1);
        }
    }
}


void SerialComm::getSerialData()
{
    qInfo() << "Reading data" << QThread::currentThread();
    qInfo() << "Bytes available" << pSerialPort->bytesAvailable();

    serialData = pSerialPort->readAll();
    for (int i = 0; i < serialData.size(); ++i) {
        ringbuffer.Insert((uint8_t) serialData.at(i));
    }

    qInfo() << "Time Elapsed:" << (float)timer.nsecsElapsed() / 1e6;
    qInfo() << "Time Elapsed:" << timer.restart();
    qInfo() << "Bytes available" << pSerialPort->bytesAvailable();

    parseData();

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
        outFileRaw.setFileName(filename + QString(".txt"));
        outFileParsed.setFileName(filename + QString("_Parsed.txt"));
        bool fileRawStatus = outFileRaw.open(QIODevice::ReadWrite);
        bool fileParsedStatus = outFileParsed.open(QIODevice::ReadWrite);
        if( !fileRawStatus && !fileParsedStatus )
        {
            qCritical() << "Could not open file!";
            qCritical() << outFileRaw.errorString();
            qCritical() << outFileParsed.errorString();
            return false;
        }
        fFileOpenStatus = true;
        return true;
    }
    else if (!Write && fFileOpenStatus) {
        // close file
        fFileOpenStatus = false;
        outFileRaw.flush();
        outFileRaw.close();
        outFileParsed.flush();
        outFileParsed.close();
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
