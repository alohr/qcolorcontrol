#include <QColorDialog>

#include <iostream>

using namespace std;

class QextSerialPort;

class ColorControl : public QColorDialog
{
    Q_OBJECT

public:
    ColorControl(QWidget *parent = 0);

private slots:
    void onCurrentColorChanged(const QColor& color); 

private:
    QextSerialPort *port_;

    QString defaultDevice();

    bool openPort();
    void sendToPort(const QColor& color);
};

