#-------------------------------------------------
#
# Project created by QtCreator 2012-09-14T11:18:05
#
#-------------------------------------------------

QT       -= gui

TARGET = QSerialPortProbe
TEMPLATE = lib
CONFIG += staticlib

VERSION = 0.0.1

SOURCES += qserialportprobe.cpp

HEADERS += qserialportprobe.h
unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
