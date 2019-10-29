#include <com-handle.h>

#include <QDebug>

    #include <sys/ioctl.h>
    #include <termios.h>
    #include <linux/serial.h>

bool ComHandle::setLowLatency()
{
    auto handle = serialPort->handle();

    if(!handle) {
        qWarning() << "Failed to get serial handle from OS.";
        return false;
    }

    // Our first attempt is with termios2
    struct termios2 {
        tcflag_t c_iflag;       /* input mode flags */
        tcflag_t c_oflag;       /* output mode flags */
        tcflag_t c_cflag;       /* control mode flags */
        tcflag_t c_lflag;       /* local mode flags */
        cc_t c_line;            /* line discipline */
        cc_t c_cc[19];          /* control characters */
        speed_t c_ispeed;       /* input speed */
        speed_t c_ospeed;       /* output speed */
    } tio2;
    if (::ioctl(handle, TCGETS2, &tio2) != -1) {
        // If it's already in low latency, no further configuration is necessary
        if (!(tio2.c_cflag & ASYNC_LOW_LATENCY)) {
            tio2.c_cflag |= ASYNC_LOW_LATENCY;
            ::ioctl(handle, TCSETS2, &tio2);
        } else {
            qDebug() << "Low latency mode is already in termios2.";
        }
    } else {
        qWarning() << "Failed to get termios2 struct from system.";
    }

    // Check again same configuration with serial_struct
    serial_struct serial;
    ::memset(&serial, 0, sizeof(serial));
    if (::ioctl(handle, TIOCGSERIAL, &serial) == -1) {
        qWarning() << "Failed to get serial_struct from system.";
        return false;
    }

    // If it's already in low latency, no further configuration is necessary
    if (serial.flags & ASYNC_LOW_LATENCY) {
        qDebug() << "Low latency mode is already in serial_struct.";
        return true;
    }

    // It's not possible to check for errors since the driver may not support it
    serial.flags |= ASYNC_LOW_LATENCY;
    ::ioctl(handle, TIOCSSERIAL, &serial);

    //light at the end of the tunnel
    return true;
}
ComHandle::ComHandle(QSerialPortInfo p)
{
    serialPort = new QSerialPort(p);
    setLowLatency();
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
