#include <QApplication>
#include <QDebug>

#include "myglwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    MyGlWidget w;
    w.show();

    return app.exec();
}
