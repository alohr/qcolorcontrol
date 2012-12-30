#include <QColorDialog>

#include <qextserialenumerator.h>
#include <qextserialport.h>

class ColorControl : public QColorDialog
{
    Q_OBJECT

public:
    ColorControl(QWidget *parent = 0);

private slots:
    void onCurrentColorChanged(const QColor& color); 
    void onDeviceDiscovered(const QextPortInfo& portInfo);
    void onDeviceRemoved(const QextPortInfo& portInfo);

private:
    QextSerialPort *port_;
    QextSerialEnumerator *portEnumerator_;

    QextSerialPort *newSerialPort(const QString& device);
    QString arduinoDevice();

    bool openPort();
    void sendToPort(const QColor& color);
};

