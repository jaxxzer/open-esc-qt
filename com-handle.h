#ifndef PINGDEVICE_H
#define PINGDEVICE_H

#include <QSerialPort>
#include <QSerialPortInfo>

class ComHandle : public QObject
{
    Q_OBJECT
public:
    ComHandle(QSerialPortInfo p);

    bool open();
    void close();

    QSerialPort* serialPort;

};

#endif // PINGDEVICE_H
