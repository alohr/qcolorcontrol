#include <QMainWindow>

#include "colorcontrol.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0) :
        QMainWindow(parent),
        colorControl_(new ColorControl(parent)) {
        setCentralWidget(colorControl_);
    }

private:
    ColorControl *colorControl_;
};
