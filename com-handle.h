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

    void write(uint8_t* data, uint16_t length);

    QSerialPort* serialPort;

signals:
    void closed();
};

#endif // PINGDEVICE_H
