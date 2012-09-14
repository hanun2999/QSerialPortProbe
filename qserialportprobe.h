#ifndef QSERIALPORTPROBE_H
#define QSERIALPORTPROBE_H

#include <QString>

#include "../QSCPIDev/qserial.h"

class QSerialPortProbe {
public:

    class Device {
    public:
        /** Supported protocols for detection. */
        typedef enum {
            SCPI,
            MANSON_PS,
            MODBUS,
            OTHER
        } Protocol;

        Device(const char* port);
        Device(const QString &port);

        /** Try open serial port and indentify device and its parametrs.
          @return True if success identify anny device, false otherwise.
          */
        bool detect();

        bool isOpenable() const;
        int bauderate() const;
        QString port() const;
        QString protocol() const;
        QString deviceName() const;
        bool isPinpointable() const;
        void pinpoint() const;
    };

    QSerialPortProbe();
    Device list();
};

#endif // QSERIALPORTPROBE_H
