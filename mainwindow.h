#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <port-scanner.h>
#include <com-handle.h>
#include <QList>
#include <QSerialPortInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
//    void on_serialComboBox_activated(QString portName);
    void on_serialConnectButton_clicked();
    void onPortScanFinished(QList<QSerialPortInfo>);

private:
    Ui::MainWindow *ui;

    PortScanner portScanner;
    ComHandle* comHandle;
};
#endif // MAINWINDOW_H
