#include <com-handle.h>

ComHandle::ComHandle(QSerialPortInfo p)
{
    serialPort = new QSerialPort(p);
}

bool ComHandle::open()
{
    return serialPort->open(QIODevice::ReadWrite);
}

void ComHandle::close()
{
    if (serialPort) {
        serialPort->close();
    }
}
