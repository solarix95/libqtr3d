#include <QApplication>
#include "viewerform.h"

#include <QSurfaceFormat>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    ViewerForm form;
    form.show();

    return app.exec();
}
