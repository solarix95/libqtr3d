#include <QApplication>
#include <QDebug>

#include "hawksim.h"
#include <libqtr3d/physics/qtr3dfpsloop.h>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dlightsource.h>
#include <libqtr3d/qtr3dassets.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w(Qtr3dWidget::MSAA4);
    w.setGeometry(50,50,2.35*500,500);

    HawkSim sim;

    w.setWindowTitle("Follow Camera: Press WASD and Arrow Keys! ← ↑ →");
    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {
        QObject::connect(&w.assets()->loop(), &Qtr3dFpsLoop::stepDone, &w, [&]() {
            w.update();
        });

        sim.init(w.assets(), w.camera());
        w.assets()->loop().setFps(50);
        w.primaryLightSource()->setPos({0,50,0});
        w.installEventFilter(&sim);
    });

    w.show();

    return app.exec();
}
