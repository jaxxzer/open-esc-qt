#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <port-scan.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&portScanner, &PortScanner::scanUpdate, this, &MainWindow::onPortScanFinished);
    portScanner.startScanning(500);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    ui->label->setText("fuck");
}

void MainWindow::on_pushButton_clicked(bool checked)
{

}

void MainWindow::onPortScanFinished(QList<QSerialPortInfo> availablePorts)
{
    ui->comboBox->clear();
    for (auto portInfo : availablePorts) {
        ui->comboBox->addItem(portInfo.portName());
    }
}
