#include <device.h>

#include <QObject>
#include <QSerialPort>

Device::Device(QSerialPortInfo info)
{

    handle = new ComHandle(info);
    connect(handle->serialPort, &QSerialPort::readyRead, this, &Device::consumeData);
}

void Device::consumeData()
{
    auto data = handle->serialPort->readAll();
    for (auto b : data) {
        parser.parseByte(b);
    }
}
