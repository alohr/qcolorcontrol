#include <QDataStream>
#include <QList>
#include <QDebug>

#include "colorcontrol.h"

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

    port_ = newSerialPort(defaultDevice());

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

    setWindowTitle(device);
    qDebug("window %s", device.toAscii().constData());

    return port;
}

QString ColorControl::defaultDevice()
{
#ifdef Q_OS_WIN
    static const QString nullDevice("NUL");
#else
    static const QString nullDevice("/dev/null");
#endif

    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    QList<QextPortInfo>::const_iterator i = ports.begin();
    while (i != ports.end()) {
	if (i->friendName.contains("Arduino", Qt::CaseInsensitive))
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
    qDebug("onDeviceDiscovered: '%s' '%s'",
	   discovered.portName.toAscii().constData(),
	   discovered.friendName.toAscii().constData());

    if (discovered.friendName.contains("Arduino", Qt::CaseInsensitive)) {
	port_->close();
	delete port_;
	port_ = newSerialPort(discovered.portName);
    }
}

void ColorControl::onDeviceRemoved(const QextPortInfo& removed)
{
    qDebug("onDeviceRemoved: removed='%s' current='%s'",
	   removed.portName.toAscii().constData(),
	   port_->portName().toAscii().constData());

    if (removed.portName.compare(port_->portName()) == 0) {
	port_->close();
	delete port_;
	port_ = newSerialPort(defaultDevice());
    }
}

