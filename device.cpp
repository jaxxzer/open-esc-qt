#include <device.h>

#include <QObject>
#include <QSerialPort>

#include <ping-message-common.h>

Device::Device(QSerialPortInfo info)
{

    handle = new ComHandle(info);
    connect(handle->serialPort, &QSerialPort::readyRead, this, &Device::consumeData);
}

bool Device::open()
{
    return handle->open();
}

void Device::requestMessage(uint16_t messageId)
{
    common_general_request msg;
    msg.set_requested_id(messageId);
    msg.updateChecksum();
    write(msg.msgData, msg.msgDataLength());
}

void Device::consumeData()
{
    auto data = handle->serialPort->readAll();
    for (auto b : data) {
        if (parser.parseByte(b) == ComParser::NEW_MESSAGE) {
            emit newData();
        }
    }
}

void requestDeviceInformation()
{

}

void Device::write(uint8_t *data, uint16_t length)
{
    if (handle) {
        handle->write(data, length);
    }
}
