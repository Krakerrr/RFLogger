#include "rfreader.h"

RFreader::RFreader(QObject *parent)
    : QObject{parent}
{
    qInfo() << this << "Constructed" << QThread::currentThread();
}

RFreader::~RFreader()
{
    qInfo() << this << "Deconstructed" << QThread::currentThread();

}
