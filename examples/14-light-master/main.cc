
#include <QApplication>
#include "viewerform.h"

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    ViewerForm form;
    form.show();

    return app.exec();
}
