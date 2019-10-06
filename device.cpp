#include <device.h>

#include <QObject>
#include <QSerialPort>

#include <ping-message-common.h>
#include <ping-message-openesc.h>
#include <register-model.h>

Device::Device(QSerialPortInfo info)
    : registerModel(this, &registerList, (uint8_t*)&deviceGlobal)
{

    handle = new ComHandle(info);
    connect(handle->serialPort, &QSerialPort::readyRead, this, &Device::consumeData);

    connect(&sendThrottleTimer, &QTimer::timeout, [&] {
//        setThrottle(_throttle);
        readRegisters();
    });
    sendThrottleTimer.start(50);

    registerList.append({0x00, "adc0", RegisterModel::REG_TYPE_UINT16});
    registerList.append({0x02, "adc1", RegisterModel::REG_TYPE_UINT16});
    registerList.append({0x04, "adc2", RegisterModel::REG_TYPE_UINT16});
    registerList.append({0x06, "adc3", RegisterModel::REG_TYPE_UINT16});
    registerList.append({0x08, "adc4", RegisterModel::REG_TYPE_UINT16});
    registerList.append({0x0a, "adc5", RegisterModel::REG_TYPE_UINT16});
    registerList.append({0x0c, "adc6", RegisterModel::REG_TYPE_UINT16});
    registerList.append({0x0e, "adc7", RegisterModel::REG_TYPE_UINT16});
    registerList.append({0x10, "throttle", RegisterModel::REG_TYPE_UINT16});


    connect(&registerModel, &RegisterModel::registerEdited, this, &Device::commitRegister);
}
void Device::readRegisters()
{
//    for (auto reg : registerList) {
//        readRegister(reg.address);
//    }
    readRegisterMulti(0x0, 18);
}

bool Device::open()
{
    return handle->open();
}

void Device::commitRegister(uint16_t index)
{
    switch (registerList[index].type) {
    case RegisterModel::REG_TYPE_BOOL:
        writeRegisterMulti(registerList[index].address, 1);
        break;
    case RegisterModel::REG_TYPE_UINT8:
        writeRegisterMulti(registerList[index].address, 1);
        break;
    case RegisterModel::REG_TYPE_UINT16:
        writeRegisterMulti(registerList[index].address, 2);
        break;
    case RegisterModel::REG_TYPE_UINT32:
        writeRegisterMulti(registerList[index].address, 4);
        break;
    }
}
void Device::writeRegister(uint16_t address, uint32_t value)
{

    openesc_set_register m;
    m.set_address(address);
    m.set_value(value);
    m.updateChecksum();
    writeMessage(m);
}

void Device::writeRegisterMulti(uint16_t address, uint16_t length)
{
    openesc_set_register_multi m(length);
    m.set_address(address);
    m.set_data_length(length);
    for (uint16_t i = 0; i < length; i++) {
        m.set_data_at(i, ((uint8_t*)&deviceGlobal)[i]);
    }
    m.updateChecksum();
    writeMessage(m);
}

void Device::writeMessage(ping_message message)
{
    handle->write(message.msgData, message.msgDataLength());
}

void Device::readRegister(uint16_t address)
{
    openesc_read_register m;
    m.set_address(address);
    m.updateChecksum();
    writeMessage(m);
}

void Device::readRegisterMulti(uint16_t address, uint16_t count)
{
    openesc_read_register_multi m;
    m.set_address(address);
    m.set_count(count);
    m.updateChecksum();
    writeMessage(m);
}
void Device::requestProtocolVersion()
{
    requestMessage(CommonId::PROTOCOL_VERSION);
}

void Device::requestDeviceInformation()
{
    requestMessage(CommonId::DEVICE_INFORMATION);
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
            handleMessage(&parser.parser.rxMessage);
            switch (parser.parser.rxMessage.message_id()) {
            case CommonId::DEVICE_INFORMATION:

                 device_type = ((common_device_information)parser.parser.rxMessage).device_type();
                break;
            default:
                break;
            }

            device_id = parser.parser.rxMessage.source_device_id();
            emit newData();
        }
    }
}

void Device::write(uint8_t *data, uint16_t length)
{
    if (handle) {
        handle->write(data, length);
    }
}

void Device::handleMessage(ping_message* message)
{
    switch (message->message_id()) {
    case CommonId::DEVICE_INFORMATION:
        device_id = message->source_device_id();
        device_type = ((common_device_information*)message)->device_type();
        break;
    case OpenescId::STATE:
    {

        openesc_state* msg = (openesc_state*)message;
        phaseA = msg->phaseA();
        phaseB = msg->phaseB();
        phaseC = msg->phaseC();
        neutral = msg->neutral();
        throttle = msg->throttle();
        voltage = msg->voltage();
        current = msg->current();
        commutationFrequency = msg->commutation_period();

    }
        break;
    case OpenescId::REGISTER:
    {
        openesc_register* m = (openesc_register*)message;
        ((uint8_t*)&deviceGlobal)[m->address()] = m->value();
        emit registerUpdate();
    }
    break;
    case OpenescId::REGISTER_MULTI:
    {
        openesc_register_multi* m = (openesc_register_multi*)message;
        for (int i = 0; i < m->data_length(); i++) {
            ((uint8_t*)&deviceGlobal)[i] = m->data()[i];
        }
        registerModel.refresh();
        emit registerUpdate();
    }
        break;
    default:
        break;

    }
    emit newData();

}
void Device::setThrottle(uint16_t throttle) {
    if (throttle > 0xfff) {
        throttle = 0xfff;
    }
    _throttle = throttle;
    openesc_set_throttle m;
    m.set_throttle_signal(_throttle);
    m.updateChecksum();
    handle->write(m.msgData, m.msgDataLength());
}
