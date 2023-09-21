#ifndef RFREADER_H
#define RFREADER_H

#include <QObject>
#include <QSerialPort>
#include <QThread>
#include <QDebug>
#include <QElapsedTimer>
#include <stdint.h>

class RFreader : public QObject
{
    Q_OBJECT

private:


public:
    // construct decostruct
    explicit RFreader(QObject *parent = nullptr);
    ~RFreader();

signals:



};

#endif // RFREADER_H
