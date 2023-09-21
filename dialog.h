#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDebug>
#include <QThread>

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

    void on_pushButton_clicked();

public slots:
    void updateGui();

signals:
    void Connect();
    void Disconnect();

private:
    Ui::Dialog *ui;
};
#endif // DIALOG_H
