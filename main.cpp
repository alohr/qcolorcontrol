#include <QApplication>

#include "colorcontrol.h"
 
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ColorControl colorControl;
    colorControl.show();

    return app.exec();
}
