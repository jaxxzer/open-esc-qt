#include <com-handle.h>

#include <QDebug>
ComHandle::ComHandle(QSerialPortInfo p)
{
    serialPort = new QSerialPort(p);
}

bool ComHandle::open()
{
    serialPort->setBaudRate(1000000);
    return serialPort->open(QIODevice::ReadWrite);

}

void ComHandle::close()
{
    if (serialPort) {
        serialPort->close();
    }
    emit closed();
}

void ComHandle::write(uint8_t* data, uint16_t length)
{
    if (serialPort) {
        serialPort->write((char*)data, length);
        serialPort->flush();
        if (serialPort->error() != QSerialPort::NoError) {
            close();
        }
    }
}
