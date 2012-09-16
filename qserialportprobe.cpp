#include "../QSCPIDev/qscpidev.h"
#include "../msdptool/src/include/msdp2xxx.h"

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
    {
        QSerial::BAUDE115200,
        Device::SCPI,
    },
    {
        QSerial::BaudeRate_t(-1),
        Device::MANSON_PS,
    },
};

QSerialPortProbe::QSerialPortProbe()
{
}

void QSerialPortProbe::detect()
{
    QStringList ports = QSerial::list();

    devices.clear();
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
    // TODO: reset all parameters
    {
        QSerial serial;
        _isOpenable = serial.open(_port, QSerial::BAUDE9600, 0, 0);
        if (!_isOpenable)
            return false;
    }

    for (int idx(0); idx < count; ++idx) {
        const Setup &setup = setups[idx];

        if (setup.protocol == SCPI) {
            QSCPIDev dev;

            if (!dev.open(_port, setup.baudeRate, false))
                continue;

            QSCPIDev::Version version = dev.systemVersion();
            if (!version.isValid())
                continue;

            if (!dev.idn(&_deviceName))
                continue;
            _isPinpointable = true;
        } else if (setup.protocol == MANSON_PS) {
            const char *portName(_port.toLocal8Bit().constData());
            sdp_t sdp;

            int sdpError = sdp_open(&sdp, portName, SDP_DEV_ADDR_MIN);
            if (sdpError != SDP_EOK) {
                continue;
            }

            sdp_va_t maximums;
            sdpError = sdp_get_va_maximums(&sdp, &maximums);
            // FIXME
            // first call is only for clean-up garabage from terminal
            sdpError = sdp_get_va_maximums(&sdp, &maximums);
            sdp_close(&sdp);
            if (sdpError != SDP_EOK) {
                continue;
            }

            QString devNameFmt("Manson SDP, limits: %1 A, %2 V");
            _deviceName = devNameFmt.arg(maximums.curr).arg(maximums.volt);

            // TODO: ...
        } else if (setup.protocol == MODBUS) {
            // TODO: ...
        } else if (setup.protocol == OTHER) {
            // skip anny protocol detection
        } else {
            // this should newer happend, should I throw exception?
            return false;
        }

        _bauderate = setup.baudeRate;
        _protocol = setup.protocol;

        return true;
    }

    // returns true if no setups supplied but port is openable
    return count == 0;
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
