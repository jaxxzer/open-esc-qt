#include <com-handle.h>

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
}

void ComHandle::write(uint8_t* data, uint16_t length)
{
    if (serialPort) {
        serialPort->write((char*)data, length);
    }
}
