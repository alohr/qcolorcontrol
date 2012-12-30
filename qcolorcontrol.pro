TEMPLATE = app

HEADERS += colorcontrol.h
SOURCES += colorcontrol.cpp main.cpp

win32 {
    CONFIG += static
}

include(3rd/qextserialport/src/qextserialport.pri)
