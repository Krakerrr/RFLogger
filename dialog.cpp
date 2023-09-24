#include "dialog.h"
#include "./ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    qInfo() << this << "Constructed" << QThread::currentThread();
    ui->setupUi(this);

    ui->lcdNumber->display("-------");


    m_StatusLabelConnectPalet.setColor(QPalette::Window, Qt::white);
    m_StatusLabelConnectPalet.setColor(QPalette::WindowText, Qt::green);
    m_StatusLabelNoConnectPalet.setColor(QPalette::Window, Qt::white);
    m_StatusLabelNoConnectPalet.setColor(QPalette::WindowText, Qt::black);
    m_StatusLabelErrorPalet.setColor(QPalette::Window, Qt::white);
    m_StatusLabelErrorPalet.setColor(QPalette::WindowText, Qt::red);

    ui->m_StatusLabel->setAutoFillBackground(true);
    ui->m_StatusLabel->setText("No Connection");
    ui->m_StatusLabel->setPalette(m_StatusLabelNoConnectPalet);

    ui->LogStatusLabel->setAutoFillBackground(true);
    ui->LogStatusLabel->setText("Waiting");
    ui->LogStatusLabel->setPalette(m_StatusLabelNoConnectPalet);

    scanSerialPorts();
}

Dialog::~Dialog()
{
    qInfo() << this << "Deconstructed" << QThread::currentThread();
    delete ui;
}


// SLOTS
void Dialog::updateGui()
{

}

void Dialog::connectionLost(QString errormsg)
{
    updateConnectStatus(ERROR);
    if( emit WriteToText("", false) )
    {
        updateLogStatus(NOTOK);
    }else
        updateLogStatus(ERROR);
}


void Dialog::updateLogStatus(Status status)
{
    if(status == OK)
    {
        ui->LogStatusLabel->setText("Writing");
        ui->LogStatusLabel->setPalette(m_StatusLabelConnectPalet);
        ui->LogBtn->setText("Stop To Text");
    }else if(status == NOTOK)
    {
        ui->LogStatusLabel->setText("Waiting");
        ui->LogStatusLabel->setPalette(m_StatusLabelNoConnectPalet);
        ui->LogBtn->setText("Write to Text");
    }else if(status == ERROR)
    {
        ui->LogStatusLabel->setText("ERROR");
        ui->LogStatusLabel->setPalette(m_StatusLabelErrorPalet);
    }
}

void Dialog::updateConnectStatus(Status status)
{
    if(status == OK)
    {
        ui->m_btn_connect->setText("Disconnect");
        ui->SerialComboBox->setDisabled(true);
        ui->RefreshSerialBtn->setDisabled(true);
        ui->m_StatusLabel->setText("Connected");
        ui->m_StatusLabel->setPalette(m_StatusLabelConnectPalet);
    }else if(status == NOTOK)
    {
        ui->m_btn_connect->setText("Connect");
        ui->SerialComboBox->setEnabled(true);
        ui->RefreshSerialBtn->setEnabled(true);
        ui->m_StatusLabel->setText("No Connection");
        ui->m_StatusLabel->setPalette(m_StatusLabelNoConnectPalet);
        scanSerialPorts();
    }else if(status == ERROR)
    {
        ui->m_btn_connect->setText("Connect");
        ui->SerialComboBox->setEnabled(true);
        ui->RefreshSerialBtn->setEnabled(true);
        ui->m_StatusLabel->setText("ERROR");
        ui->m_StatusLabel->setPalette(m_StatusLabelErrorPalet);
        scanSerialPorts();
    }
}



// Buttons Signal

void Dialog::scanSerialPorts()
{
    ui->SerialComboBox->clear();
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
        ui->SerialComboBox->addItem( port.portName());
    }
}


void Dialog::on_m_btn_connect_clicked()
{
    if( ui->m_btn_connect->text() == "Connect")
    {
        bool connectStatus = false;
        connectStatus = emit Connect(ui->SerialComboBox->currentText());
        if(!connectStatus)
        {
            QMessageBox::information(this,"Serial Port Error\n" , "Could not open serial port");
            updateConnectStatus(ERROR);
        }
        else
        {
            updateConnectStatus(OK);
        }
    }else
    {
        emit Disconnect();
        updateConnectStatus(NOTOK);
    }

}


void Dialog::on_RefreshSerialBtn_clicked()
{
    scanSerialPorts();
}


void Dialog::on_SerialComboBox_currentTextChanged(const QString &arg1)
{
    qInfo() << this << ui->SerialComboBox->currentText();
}


void Dialog::on_LogBtn_clicked()
{
    static int fileNoCount = 1;
    QString filename = QString("Log") + QString::number(fileNoCount);
    if(ui->LogBtn->text() == "Write to Text")
    {
        if( emit WriteToText(filename, true) )
        {
            updateLogStatus(OK);
            fileNoCount++;
        }else
            updateLogStatus(ERROR);
    }
    else
    {
        if( emit WriteToText(filename, false) )
        {
            updateLogStatus(NOTOK);
        }else
            updateLogStatus(ERROR);
    }
}

