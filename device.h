#ifndef DEVICE_H
#define DEVICE_H

#include <com-handle.h>
#include <com-parser.h>
#include <QSerialPort>
#include <QObject>
#include <inttypes.h>
class Device : public QObject
{
    Q_OBJECT
public:
    Device(QSerialPortInfo info);
    ComParser parser;
    ComHandle* handle;
    bool open();

    void requestDeviceInformation();
    void requestProtocolVersion();
    void requestMessage(uint16_t messageId);

    void consumeData();

    uint8_t device_type;
    uint8_t device_id;

    void handleMessage(ping_message* message);

    uint16_t phaseA, phaseB, phaseC, neutral, current, voltage, throttle, commutationFrequency;

private:
    void write(uint8_t* data, uint16_t length);

signals:
    void newData();
};

#endif // DEVICE_H
