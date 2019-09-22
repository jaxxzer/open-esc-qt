#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , device(nullptr)
{
    ui->setupUi(this);

    connect(&portScanner, &PortScanner::scanUpdate, this, &MainWindow::onPortScanFinished);
    portScanner.startScanning(500);

    ui->customPlot->addGraph();

}

MainWindow::~MainWindow()
{
    delete ui;
    if (device) {
        delete device;
    }
}


void MainWindow::on_serialConnectButton_clicked()
{
    for (auto availablePort : portScanner.availablePorts) {
        if (availablePort.portName() == ui->serialComboBox->currentText()) {
            if (!device) {
                portScanner.stopScanning();
                device = new Device(availablePort);
                connect(device, &Device::newData, this, &MainWindow::handleNewDeviceData);
                if (device->open()) {
                    ui->label->setText(availablePort.portName());
                    ui->serialConnectButton->setText("disconnect");
                    device->requestDeviceInformation();
                }
            }
        }
    }
}

void MainWindow::handleNewDeviceData()
{
    ui->label->setText(QString::number(device->phaseA));
}

void MainWindow::onPortScanFinished(QList<QSerialPortInfo> availablePorts)
{
    ui->serialComboBox->clear();
    for (auto portInfo : availablePorts) {
        ui->serialComboBox->addItem(portInfo.portName());
    }
}
