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
    void onDeviceDiscovered(const QextPortInfo&);
    void onDeviceRemoved(const QextPortInfo& removed);

private:
    QextSerialPort *port_;
    QextSerialEnumerator *portEnumerator_;

    QextSerialPort *newSerialPort(const QString& device);
    void closeSerialPort(QextSerialPort *port);

    QString arduinoDevice();

    bool openPort();
    void sendToPort(const QColor& color);
};

