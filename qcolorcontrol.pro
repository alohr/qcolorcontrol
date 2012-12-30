TEMPLATE = app

HEADERS += colorcontrol.h
SOURCES += colorcontrol.cpp main.cpp

CONFIG += static console

include(3rd/qextserialport/src/qextserialport.pri)
