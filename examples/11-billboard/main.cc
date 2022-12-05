#include <QApplication>
#include <QDebug>
#include <QSurfaceFormat>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dtexturedmesh.h>
#include <libqtr3d/qtr3dcameracycler.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dmodelfactory.h>
#include <libqtr3d/qtr3dgeometrybufferstate.h>
#include <libqtr3d/qtr3dbillboard.h>
#include <libqtr3d/debug/qtr3dfreecameracontroller.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w;
    w.setGeometry(50,50,500,500);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {

        auto *buffer = w.createTexturedMesh(":/sun.png");
        buffer->startMesh();
        buffer->addQuad({-1, 1, 0}, { 1, 1, 0}, { 1,-1, 0}, {-1,-1, 0});  // Front
        buffer->endMesh();

        auto *sunState = w.createBufferState(buffer);
        sunState->setLightingType(Qtr3d::NoLighting);
        new Qtr3dBillboard(w.camera(),sunState);

        // Sky
        auto *mesh = w.createVertexMesh();
        Qtr3dModelFactory::meshByStarsky(*mesh,1000,1000,Qt::white);
        w.createBufferState(mesh, Qtr3d::NoLighting);

        w.camera()->lookAt({0,0,12},{0,0,0},{0,1,0});
        new Qtr3dFreeCameraController(&w);
        // new Qtr3dCameraCycler(w.camera(),30,{0.3,0.3,0.3},{0,0,-12},{0,0,0});
    });

    w.show();

    return app.exec();
}
