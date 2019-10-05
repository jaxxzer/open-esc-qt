#ifndef DEVICE_H
#define DEVICE_H

#include <com-handle.h>
#include <com-parser.h>
#include <QSerialPort>
#include <QObject>
#include <QTimer>
#include <QMap>

#include <register-model.h>

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
    void setThrottle(uint16_t throttle);

    void writeRegister(uint16_t address, uint32_t* data);
    void readRegister(uint16_t address, uint32_t* data);

    void consumeData();


    uint8_t device_type;
    uint8_t device_id;

    void handleMessage(ping_message* message);

    uint16_t phaseA, phaseB, phaseC, neutral, current, voltage, throttle, commutationFrequency;

private:
    uint16_t _throttle;
    void write(uint8_t* data, uint16_t length);
    QTimer sendThrottleTimer;
    QVector<RegisterModel::register_t> registerList;

    RegisterModel registerModel;


signals:
    void newData();
};

#endif // DEVICE_H
