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

signals:
    void newData(QByteArray data);
};

#endif // PINGDEVICE_H
