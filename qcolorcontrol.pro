TEMPLATE = app

HEADERS += colorcontrol.h
SOURCES += colorcontrol.cpp main.cpp

QT += widgets

win32 {
    CONFIG += static
}

linux* {
    CONFIG += qesp_linux_udev
}

include(3rd/qextserialport/src/qextserialport.pri)
