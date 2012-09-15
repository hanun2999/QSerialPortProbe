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
        const QString &deviceName() const;

        /** Return true if open() suceeds for serial port. */
        bool isOpenable() const;

        /** Return true if device supports pinpointing like beeping, led flashing
          or other king of noise.
          */
        bool isPinpointable() const;

        /** Make device identify yourself, if supported.

          Make beep or flash some led or so, depends on device type.
          */
        void pinpoint() const;

        /** Return serial port name. */
        const QString &port() const;

        /** Return detected protocol. */
        Protocol protocol() const;

        /** Return string representation of protocol. */
        const QString protocolString() const;

    protected:
        QSerial::BaudeRate_t _bauderate;
        bool _isPinpointable;
        QString _port;
        Protocol _protocol;
    };

    QSerialPortProbe();
    QVector<Device> list();

protected:
    const static Device::Setup defaultSetups[];
};

#endif // QSERIALPORTPROBE_H
