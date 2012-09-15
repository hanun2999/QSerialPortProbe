#ifndef QSERIALPORTPROBE_H
#define QSERIALPORTPROBE_H

#include <QString>

#include "../QSCPIDev/qserial.h"

class QSerialPortProbe {
public:
    class Device {
    public:
        /** Supported protocols. */
        typedef enum {
            NONE,
            SCPI,
            MANSON_PS,
            MODBUS,
            OTHER
        } Protocol;

        /** Setup for device detection. */
        struct Setup {
            QSerial::BaudeRate_t baudeRate;
            Protocol protocol;
        };

        /** Initialize new device detector for specified serial port. */
        Device(const char* port);
        
        /** Initialize new device detector for specified serial port. */
        Device(const QString &port);

        /** Return detected bauderate . */
        QSerial::BaudeRate_t bauderate() const;

        /** Try open serial port and indentify device and its parameters.
          @return True if success identify anny device, false otherwise.
          */
        bool detect(const Setup setups[]);

        /** Return device type/name/description. */
        inline const QString &deviceName() const
        {
            return _deviceName;
        }

        /** Return true if open() suceeds for serial port. */
        inline bool isOpenable() const
        {
            return _isOpenable;
        }

        /** Return true if device supports pinpointing like beeping, led flashing
          or other king of noise.
          */
        bool isPinpointable() const;

        /** Make device identify yourself, if supported.

          Make beep or flash some led or so, depends on device type.
          */
        void pinpoint() const;

        /** Return serial port name. */
        inline const QString &port() const
        {
            return _port;
        }

        /** Return detected protocol. */
        inline Protocol protocol() const
        {
            return _protocol;
        }

        /** Return string representation of protocol. */
        QString protocolString() const;

    protected:
        QSerial::BaudeRate_t _bauderate;
        QString _deviceName;
        bool _isOpenable;
        bool _isPinpointable;
        QString _port;
        Protocol _protocol;
    };

    typedef QVector<Device> DeviceList;

    QSerialPortProbe();

    /** Start device detection. */
    void detect();

    /** Return list of detected devices. */
    const DeviceList &list() const
    {
        return devices;
    }

protected:
    DeviceList devices;
    const static Device::Setup defaultSetups[];
};

#endif // QSERIALPORTPROBE_H
