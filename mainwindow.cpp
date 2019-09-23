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
    ui->customPlot->addGraph();
    ui->customPlot->addGraph();
    ui->customPlot->addGraph();
    ui->customPlot->addGraph();
    ui->customPlot->addGraph();
    ui->customPlot->addGraph();
    ui->customPlot->addGraph();

    ui->customPlot->graph(0)->setPen(QColor(Qt::red));
    ui->customPlot->graph(1)->setPen(QColor(Qt::blue));
    ui->customPlot->graph(2)->setPen(QColor(Qt::cyan));
    ui->customPlot->graph(3)->setPen(QColor(Qt::green));
    ui->customPlot->graph(4)->setPen(QColor(Qt::black));
    ui->customPlot->graph(5)->setPen(QColor(Qt::magenta));
    ui->customPlot->graph(6)->setPen(QColor(Qt::gray));
    ui->customPlot->graph(7)->setPen(QColor(Qt::darkYellow));
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

        ui->customPlot->graph(0)->addData(key, device->phaseA);
        ui->customPlot->graph(1)->addData(key, device->phaseB);
        ui->customPlot->graph(2)->addData(key, device->phaseC);
        ui->customPlot->graph(3)->addData(key, device->neutral);

//        ui->customPlot->graph(4)->addData(key, device->current);
//        ui->customPlot->graph(4)->rescaleValueAxis(false, true);

////        ui->customPlot->graph(5)->addData(key, device->voltage);
//        ui->customPlot->graph(6)->addData(key, device->throttle);
//        ui->customPlot->graph(6)->rescaleValueAxis(false, true);

//        ui->customPlot->graph(7)->addData(key, device->commutationFrequency);



            if (key-lastPointKey > 0.010) // at most add point every 2 ms
            {

        ui->customPlot->xAxis->setRange(key, 5, Qt::AlignRight);

        ui->customPlot->graph(0)->rescaleValueAxis(false, true);
        ui->customPlot->graph(1)->rescaleValueAxis(true, true);
        ui->customPlot->graph(2)->rescaleValueAxis(true, true);
        ui->customPlot->graph(3)->rescaleValueAxis(true, true);

        ui->customPlot->replot(QCustomPlot::rpQueuedReplot);
        lastPointKey = key;

    }

}

void MainWindow::onPortScanFinished(QList<QSerialPortInfo> availablePorts)
{
    ui->serialComboBox->clear();
    for (auto portInfo : availablePorts) {
        ui->serialComboBox->addItem(portInfo.portName());
    }
}
