
#include <QApplication>
#include <QDebug>

#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dfactory.h>
#include <libqtr3d/qtr3dgeometrybufferstate.h>
#include <libqtr3d/qtr3dbillboard.h>
#include <libqtr3d/debug/qtr3dfreecameracontroller.h>

/*

                     SORRY - NOT YET IMPLEMENTED

*/
int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w;
    w.setGeometry(50,50,500,500);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {

        auto *sun = w.createMesh();

        sun->startMesh(Qtr3d::Triangle);
        sun->setTexture(QImage(":/sun.png"));
        sun->addQuad({-1, 1, 0}, { 1, 1, 0}, { 1,-1, 0}, {-1,-1, 0});  // Front
        sun->endMesh();

        auto *sunState = w.createState(sun);
        sunState->setLightingType(Qtr3d::NoLighting);
        new Qtr3dBillboard(w.camera(),sunState);

        // Sky
        auto *sky = w.createMesh();
        Qtr3d::meshByStarsky(*sky,1000,1000,Qt::white);
        w.createState(sky, Qtr3d::NoLighting);

        w.camera()->lookAt({0,0,12},{0,0,0},{0,1,0});
        new Qtr3dFreeCameraController(&w);
    });

    w.show();

    return app.exec();
}
