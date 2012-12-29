qcolorcontrol
=============

## Get Qt

## Download [qextserialport](http://code.google.com/p/qextserialport/)

The qcolorcontrol.pro file is set up to expect the qextserialport
library in 3rd/qextserialport:

```
include(3rd/qextserialport/src/qextserialport.pri)
```

Grab qextserialport:

```
git clone https://code.google.com/p/qextserialport/ 3rd/qextserialport

```

## Build

```
qmake
make
```




