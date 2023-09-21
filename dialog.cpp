#include "dialog.h"
#include "./ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    qInfo() << this << "Constructed" << QThread::currentThread();
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    qInfo() << this << "Deconstructed" << QThread::currentThread();
    delete ui;
}


void Dialog::on_m_btn_connect_clicked()
{
    emit Connect();
}

void Dialog::updateGui()
{

}


void Dialog::on_pushButton_clicked()
{
    emit Disconnect();
}

