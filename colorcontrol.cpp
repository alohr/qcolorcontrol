#include <QDataStream>
#include <QList>
#include <QDebug>

#include "colorcontrol.h"

static const int ARDUINO_USB_VENDOR_ID = 9025;


ColorControl::ColorControl(QWidget *parent) : QColorDialog(parent)
{
    setOption(QColorDialog::NoButtons);
#ifdef DONT_USE_NATIVE_DIALOG
    setOption(QColorDialog::DontUseNativeDialog);
#endif

    portEnumerator_.setUpNotifications();

    connect(&portEnumerator_, SIGNAL(deviceDiscovered(const QextPortInfo &)),
	    this, SLOT(onDeviceDiscovered(const QextPortInfo &)));
    connect(&portEnumerator_, SIGNAL(deviceRemoved(const QextPortInfo &)),
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

    return port;
}

void ColorControl::closeSerialPort(QextSerialPort *port)
{
    if (port) {
	port->close();
	delete port;
    }
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
	       i->portName.toLatin1().constData(),
	       i->friendName.toLatin1().constData(),
	       i->vendorID,
	       i->productID);
	if (i->portName.length() > 0 &&
	    (i->vendorID == ARDUINO_USB_VENDOR_ID ||
	     i->friendName.contains("Arduino", Qt::CaseInsensitive)))
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
    qDebug("%s", colorName.toLatin1().constData());

    int n = port_->write(colorName.toLatin1(),
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

void ColorControl::onDeviceDiscovered(const QextPortInfo&)
{
    QString device = arduinoDevice();
    if (device.compare(port_->portName()) != 0) {
	closeSerialPort(port_);
	port_ = newSerialPort(device);
    }
}

void ColorControl::onDeviceRemoved(const QextPortInfo& removed)
{
    if (removed.portName.compare(port_->portName()) == 0) {
	closeSerialPort(port_);
	port_ = newSerialPort(arduinoDevice());
    }
}

