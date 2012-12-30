#include <QDataStream>
#include <QList>
#include <QDebug>

#include "colorcontrol.h"

static const int ARDUINO_VENDOR_ID = 9025;

ColorControl::ColorControl(QWidget *parent) : QColorDialog(parent)
{
    setOption(QColorDialog::NoButtons);
    setModal(true);

    portEnumerator_ = new QextSerialEnumerator();
    portEnumerator_->setUpNotifications();

    connect(portEnumerator_, SIGNAL(deviceDiscovered(const QextPortInfo &)),
	    this, SLOT(onDeviceDiscovered(const QextPortInfo &)));
    connect(portEnumerator_, SIGNAL(deviceRemoved(const QextPortInfo &)),
	    this, SLOT(onDeviceRemoved(const QextPortInfo &)));

    port_ = newSerialPort(arduinoDevice());

    connect(this, SIGNAL(currentColorChanged(QColor)),
	    this, SLOT(onCurrentColorChanged(QColor)));
}

QextSerialPort *ColorControl::newSerialPort(const QString& device)
{
    QextSerialPort *port = new QextSerialPort(device, QextSerialPort::Polling);

    port->setBaudRate(BAUD115200);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_1);
    port->setTimeout(500);

    setWindowTitle("Arduino Port " + device);
    qDebug("window %s", device.toAscii().constData());

    return port;
}

QString ColorControl::arduinoDevice()
{
#ifdef Q_OS_WIN
    static const QString nullDevice("NUL");
#else
    static const QString nullDevice("/dev/null");
#endif

    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    QList<QextPortInfo>::const_iterator i = ports.begin();
    while (i != ports.end()) {
		qDebug("device: port=%s friendly=%s vendor=%d product=%d",
			i->portName.toAscii().constData(),
			i->friendName.toAscii().constData(),
			i->vendorID,
			i->productID);
		if (i->vendorID == ARDUINO_VENDOR_ID ||
		    i->friendName.contains("Arduino", Qt::CaseInsensitive))
			return i->portName;
		++i;
    }

    return nullDevice;
}

bool ColorControl::openPort()
{
    if (!port_->isOpen())
	port_->open(QIODevice::ReadWrite | QIODevice::Unbuffered);

    return port_->isOpen();
}

void ColorControl::sendToPort(const QColor& color)
{
    QString colorName = color.name();
    qDebug("%s", colorName.toAscii().constData());

    int n = port_->write(colorName.toAscii(),
			 colorName.length());
    if (n < 0)
	port_->close();
}

void ColorControl::onCurrentColorChanged(const QColor& color)
{
    if (!openPort())
	return;

    sendToPort(color);
}

void ColorControl::onDeviceDiscovered(const QextPortInfo& discovered)
{
    qDebug("onDeviceDiscovered: '%s' '%s' vendor=%d product=%d",
	   discovered.portName.toAscii().constData(),
	   discovered.friendName.toAscii().constData(),
       discovered.vendorID,
       discovered.productID);

    QString port = arduinoDevice();
    if (port.compare(port_->portName()) != 0) {
    	port_->close();
	    delete port_;
	    port_ = newSerialPort(port);
    }
}

void ColorControl::onDeviceRemoved(const QextPortInfo& removed)
{
    qDebug("onDeviceRemoved: removed='%s' current='%s' vendor=%d product=%d",
	   removed.portName.toAscii().constData(),
	   port_->portName().toAscii().constData(),
       removed.vendorID,
       removed.productID);

    if (removed.portName.compare(port_->portName()) == 0) {
	port_->close();
	delete port_;
	port_ = newSerialPort(arduinoDevice());
    }
}

