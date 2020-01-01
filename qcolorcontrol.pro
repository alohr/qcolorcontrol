TEMPLATE = app

HEADERS += colorcontrol.h mainwindow.h
SOURCES += colorcontrol.cpp main.cpp

QT += widgets

win32 {
    CONFIG += static
}

linux* {
    CONFIG += qesp_linux_udev
}

macx {
    DEFINES += DONT_USE_NATIVE_DIALOG
}

include(3rd/qextserialport/src/qextserialport.pri)
