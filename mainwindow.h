#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <qcustomplot.h>
#include <port-scanner.h>
#include <device.h>
#include <QList>
#include <QSerialPortInfo>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    typedef enum {
        DISCONNECTED,
        CONNECTED,
    } connect_e;

    connect_e connectState;
    void deviceClosed();

private slots:
//    void on_serialComboBox_activated(QString portName);
    void on_serialConnectButton_clicked();
    void onPortScanFinished(QList<QSerialPortInfo>);

private:
    Ui::MainWindow *ui;

    PortScanner portScanner;
    Device* device;

    QVector<double> x, phaseASamples, phaseBSamples, phaseCSamples;
    void handleNewDeviceData();

    QTimer replotTimer;

    void readDeviceRegisters();
};
#endif // MAINWINDOW_H
