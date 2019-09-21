#ifndef PORTSCANNER_H
#define PORTSCANNER_H

#include <QSerialPortInfo>
#include <QTimer>
#include <QObject>
#include <QList>

class PortScanner : public QObject
{
    Q_OBJECT
public:
    PortScanner();

    void startScanning(uint16_t periodMs = 500);
    void stopScanning();
    void checkPorts();
    QList<QSerialPortInfo> availablePorts;
    QTimer timer;

signals:
    void scanUpdate(QList<QSerialPortInfo> availablePorts);
};

#endif // PORTSCANNER_H
