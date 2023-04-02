#include <QApplication>
#include <QDebug>
#include "mainview.h"

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    MainView w;

    w.setGeometry(50,50,500,500);
    w.setOptions(Qtr3dWidget::MSAA16);

    w.show();

    return app.exec();
}
