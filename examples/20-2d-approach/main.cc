#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    MainWindow w;
    w.setGeometry(50,50,500,500);
    w.setOptions(Qtr3dWidget::MSAA16);
    w.show();

    return app.exec();
}
