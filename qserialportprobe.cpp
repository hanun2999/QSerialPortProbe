#include "qserialportprobe.h"


QSerialPortProbe::QSerialPortProbe()
{
}

QSerialPortProbe::Device::Device(const char *port) :
    _bauderate(QSerial::BaudeRate_t(-1)),
    _isOpenable(false),
    _isPinpointable(false),
    _port(port),
    _protocol(NONE)
{

}

QSerialPortProbe::Device::Device(const QString &port) :
    _bauderate(QSerial::BaudeRate_t(-1)),
    _isOpenable(false),
    _isPinpointable(false),
    _port(port),
    _protocol(NONE)
{ }

QString QSerialPortProbe::Device::protocolString() const
{
    switch(_protocol) {
    default:
    case NONE:
        return "NONE";
    case SCPI:
        return "SCPI";
    case OTHER:
        return "unknown";
    case MANSON_PS:
        return "Manson PS";
    case MODBUS:
        return "ModBus";
    }
}
