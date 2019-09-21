#include <port-scan.h>

PortScanner::PortScanner()
{
    connect(&timer, &QTimer::timeout, this, &PortScanner::checkPorts);
}

void PortScanner::startScanning(uint16_t periodMs)
{
    timer.start(periodMs);
}

void PortScanner::checkPorts()
{
    availablePorts = QSerialPortInfo::availablePorts();
    emit scanUpdate(availablePorts);
}
