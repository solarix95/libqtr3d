#include <QApplication>
#include <QDebug>

#include "hawksim.h"
#include <libqtr3d/physics/qtr3dfpsloop.h>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dcontext.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w(Qtr3dWidget::MSAA4);
    w.setGeometry(50,50,2.35*500,500);

    HawkSim sim;

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {
        QObject::connect(&w.bufferContext()->loop(), &Qtr3dFpsLoop::stepDone, &w, [&]() {
            w.update();
        });

        sim.init(w.bufferContext(), w.camera());
        w.bufferContext()->loop().setFps(50);
    });

    w.show();

    return app.exec();
}
