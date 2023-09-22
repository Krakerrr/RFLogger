#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDebug>
#include <QThread>
#include <QSerialPortInfo>
#include <QMessageBox>

enum Status { OK=0,NOTOK, ERROR};

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_m_btn_connect_clicked();
    void scanSerialPorts();
    void on_RefreshSerialBtn_clicked();
    void on_SerialComboBox_currentTextChanged(const QString &arg1);
    void on_LogBtn_clicked();

public slots:
    void updateGui();
    void connectionLost(QString errormsg);

signals:
    bool Connect(QString ComPort);
    void Disconnect();
    bool WriteToText(QString filename, bool Write);

private:
    Ui::Dialog *ui;
    QPalette m_StatusLabelConnectPalet;
    QPalette m_StatusLabelNoConnectPalet;
    QPalette m_StatusLabelErrorPalet;
    void updateLogStatus(Status status);
    void updateConnectStatus(Status status);

};
#endif // DIALOG_H
