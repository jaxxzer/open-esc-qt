#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <register-model.h>

#include <QRandomGenerator>

#include <qcustomplot.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , device(nullptr)
    , replotTimer(this)
    , connectState(CONNECT)
{
    ui->setupUi(this);

    connect(ui->horizontalSlider, &QSlider::valueChanged, device, &Device::setThrottle);
    connect(ui->horizontalSlider, &QSlider::valueChanged, [&](int value) {
        ui->label_3->setText(QString::number(value));
        device->setThrottle(value);
    });

    connect(&portScanner, &PortScanner::scanUpdate, this, &MainWindow::onPortScanFinished);
    portScanner.startScanning(500);

    for (uint8_t i = 0; i < 20; i++) {
        ui->customPlot->addGraph()->setVisible(false);
        // random color for each plot
        ui->customPlot->graph(i)->setPen(QColor(QRandomGenerator::global()->bounded(50,255), QRandomGenerator::global()->bounded(50, 255), QRandomGenerator::global()->bounded(50,255)));
    }

    ui->customPlot->yAxis2->setVisible(true);
}

void MainWindow::onPlotEnabledChanged(int index) {
    if (device->registerList[index].plotEnabled) {
        ui->customPlot->graph(index)->setVisible(true);
    } else {
        ui->customPlot->graph(index)->setVisible(false);
    }
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
    if (connectState == CONNECT) {
        for (auto availablePort : portScanner.availablePorts) {
            if (availablePort.portName() == ui->serialComboBox->currentText()) {
                if (!device) {
                    portScanner.stopScanning();
                    device = new Device(availablePort);
                    connect(device, &Device::newData, this, &MainWindow::handleNewDeviceData);
                    connect(device, &Device::closed, this, &MainWindow::deviceClosed);
                    connect(&device->registerModel, &RegisterModel::plotEnabledChanged, this, &MainWindow::onPlotEnabledChanged);

                    if (device->open()) {
                        ui->label->setText(availablePort.portName());
                        ui->serialConnectButton->setText("disconnect");
                        device->requestDeviceInformation();
                        replotTimer.start(40);
                        ui->tableView->setModel(device->getRegisterModel());
//                        device->readRegisters();
                        device->readRegisterMulti(0x0, 22);


                        connectState = DISCONNECT;
                    }
                }
            }
        }

    } else if (connectState == DISCONNECT) {
        device->close();
    }

    //assert_not_reached();

}

void MainWindow::deviceClosed()
{
    if (device) {
        delete(device);
        device = nullptr;
    }

    portScanner.startScanning();
    ui->serialConnectButton->setText("connect");
    connectState = CONNECT;
}

void MainWindow::readDeviceRegisters()
{

//    ui->tableView->setModel();
}

void MainWindow::handleNewDeviceData()
{
    ui->label->setText(QString::number(device->commutationFrequency));

    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;

            for (int i = 0; i < device->registerList.size(); i++) {
                ui->customPlot->graph(i)->addData(key, device->deviceGlobal.adc_buffer[i]);
            }

            if (key-lastPointKey > 0.020) { // at most add point every 2 ms

                ui->customPlot->xAxis->setRange(key, 5, Qt::AlignRight);

                bool first = true;
                for (int i = 0;i < device->registerList.size(); i++) {
                    if (device->registerList[i].plotEnabled) {
                        if (first) {
                            ui->customPlot->graph(i)->rescaleValueAxis(false, true);
                            first = false;
                        } else {
                            ui->customPlot->graph(i)->rescaleValueAxis(true, true);
                        }
                    }
                }

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
