#include <QDataStream>
#include <QDir>

#include <qextserialport.h>

using namespace std;

#include "colorcontrol.h"

static QextSerialPort *newSerialPort(const QString& name)
{
    QextSerialPort *port = new QextSerialPort(name, QextSerialPort::Polling);

    port->setBaudRate(BAUD115200);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_1);
    port->setTimeout(500);

    return port;
}
    
ColorControl::ColorControl(QWidget *parent) : QColorDialog(parent)
{
    setOption(QColorDialog::NoButtons);
    setModal(true);

    connect(this, SIGNAL(currentColorChanged(QColor)),
	    this, SLOT(onCurrentColorChanged(QColor))); 

    QString device(defaultDevice());

    port_ = newSerialPort(device);
    setWindowTitle(device);
}

QString ColorControl::defaultDevice()
{
    static const QString nullDevice("/dev/null");

    QDir dir("/dev");
    QStringList filter("tty.usbmodem*");
    QStringList devices = dir.entryList(filter, QDir::System);

    return devices.empty() ? nullDevice : devices[0];
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
