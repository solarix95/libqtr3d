#include <QApplication>
#include <QDebug>
#include "mainview.h"
#include "viewerform.h"

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    ViewerForm f;
    f.show();

    return app.exec();
}
