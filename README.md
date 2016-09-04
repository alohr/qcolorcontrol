qcolorcontrol
=============

## Get Qt

## Download [qextserialport](https://github.com/qextserialport/qextserialport)

The qcolorcontrol.pro file is set up to expect the qextserialport
library in 3rd/qextserialport:

```
include(3rd/qextserialport/src/qextserialport.pri)
```

Grab qextserialport:

```
git clone https://github.com/qextserialport/qextserialport.git 3rd/qextserialport
```

## Build

```
qmake
make
```




