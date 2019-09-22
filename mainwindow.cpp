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
    ui->customPlot->graph(0)->setPen(QColor(Qt::red));
    QVector <double> x(200), y(200);
    for (uint16_t i = 0; i < 200; i++) {
        x[i] = i;
        y[i] = 2*i;
    }
    ui->customPlot->graph(0)->setData(x, y);
    ui->customPlot->graph(0)->rescaleAxes();
    ui->customPlot->replot();
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

    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
        ui->customPlot->graph(0)->addData(key, device->phaseA);
        lastPointKey = key;
        ui->customPlot->yAxis->rescale();
        ui->customPlot->xAxis->setRange(key, 5, Qt::AlignRight);
        ui->customPlot->replot();
    }

}

void MainWindow::onPortScanFinished(QList<QSerialPortInfo> availablePorts)
{
    ui->serialComboBox->clear();
    for (auto portInfo : availablePorts) {
        ui->serialComboBox->addItem(portInfo.portName());
    }
}
