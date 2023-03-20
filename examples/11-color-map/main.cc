#include <QApplication>
#include <QSurfaceFormat>
#include <QDebug>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dmesh.h>
#include <libqtr3d/qtr3dcameracycler.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dlightsource.h>
#include <libqtr3d/qtr3dfactory.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w(Qtr3dWidget::MSAA16);

    w.setGeometry(10,10,500,500);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {
        qDebug() << "OpenGL ready";

        // Sphere
        auto *sphere = w.createMesh();
        Qtr3d::meshBySphere(*sphere,60,QImage(":/planet.jpg"));
        // sphere->setMeshType(Qtr3dGeometryBuffer::Dot);
        sphere->setFaceOrientation(Qtr3d::CounterClockWise);
        auto *state = w.createState(sphere);
        state->setLightingType(Qtr3d::PhongLighting);
        state->setState({0,0,0},{-90,0,0},{1,1,1});

        // Sky
        auto *mesh = w.createMesh();
        Qtr3d::meshByStarsky(*mesh,1000,1000,Qt::white);
        w.createState(mesh)->setLightingType(Qtr3d::NoLighting);

        mesh = w.createMesh();
        Qtr3d::meshByStarsky(*mesh,1000,100,Qt::blue);
        w.createState(mesh);

        /*
        auto *debugMesh = w.createMesh();
        Qtr3d::normalMeshByMesh(*debugMesh,*sphere,1);
        auto *debugState = w.createBufferState(debugMesh);
        debugState->setState({0,0,0},{0,0,0},{1,1,1});
        */

        w.primaryLightSource()->setPos({5,0,5});
        new Qtr3dCameraCycler(w.camera(),30,0.3,{0.0,0.0,float(5*sphere->radius())},{0,0,0});
    });

    w.show();

    return app.exec();
}
