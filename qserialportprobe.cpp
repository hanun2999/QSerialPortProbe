#include "qserialportprobe.h"


#define ARRAY_ITEMS(array) ((sizeof(array)/sizeof(array[0])))

const QSerialPortProbe::Device::Setup QSerialPortProbe::defaultSetups[] = {
    {
        QSerial::BAUDE9600,
        Device::SCPI,
    },
    {
        QSerial::BAUDE19200,
        Device::SCPI,
    },
};

QSerialPortProbe::QSerialPortProbe()
{
}

void QSerialPortProbe::detect()
{
    QStringList ports = QSerial::list();

    foreach(const QString &port, ports) {
        Device device(port);

        if (device.detect(defaultSetups, ARRAY_ITEMS(defaultSetups))) {
            devices.push_back(device);
        }
    }
}

QSerialPortProbe::Device::Device(const char* port) :
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

bool QSerialPortProbe::Device::detect(const Setup setups[], int count)
{
    for (int idx(0); idx < count; ++idx) {
        QSerial serial;

        if (!serial.open(_port, setups[idx].baudeRate, 100000, 0))
            continue;

        // TODO: ...
        _bauderate = setups[idx].baudeRate;
        return true;
    }

    return false;
}

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
