#include <QApplication>
#include <QDebug>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dmesh.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dfactory.h>
#include <libqtr3d/qtr3dlightsource.h>
#include <libqtr3d/extras/qtr3dorbitcameracontroller.h>
#include "solarsystem.h"

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w;
    w.setGeometry(10,10,500,500);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {

        w.setDefaultLighting(Qtr3d::FlatLighting);
        w.primaryLightSource()->setAmbientStrength(0.3);
        const int itemsCount = 4;
        QVector3D showRoomPos(0,0,-3);
        QMatrix4x4 showRoomRotate;
        showRoomRotate.rotate(360/itemsCount,{0,1,0});

        Qtr3dMesh *mesh;
        Qtr3dGeometryState *state;

        //mesh = w.createVertexMesh();
        //Qtr3dModelFactory::meshBySphere(*mesh,13,Qt::red);
        //w.createBufferState(mesh)->setFlat(false);

        mesh = w.createMesh();
        Qtr3d::meshByXyzAxis(*mesh);
        w.createState(mesh, Qtr3d::NoLighting);

        // Cycle
        mesh = w.createMesh();
        Qtr3d::meshByCycle(*mesh,13,Qt::red);
        state = w.createState(mesh);
        state->setState(showRoomPos,{0,0,45});
        showRoomPos = showRoomRotate * showRoomPos;

        // Sphere
        mesh = w.createMesh();
        Qtr3d::meshBySphere(*mesh,13,Qt::red);
        state = w.createState(mesh);
        state->setState(showRoomPos,{0,0,0});
        showRoomPos = showRoomRotate * showRoomPos;

        // Cylinder
        mesh = w.createMesh();
        Qtr3d::meshByCylinder(*mesh,13,false,false,Qt::red);
        state = w.createState(mesh);
        state->setState(showRoomPos,{0,0,0});
        showRoomPos = showRoomRotate * showRoomPos;

        // Solar System
        //SolarSystem *solar = new SolarSystem(w.textures(),&w);
        //w.createBufferState(solar)->setState(showRoomPos,{0,0,0},{0.1,0.1,0.1});
        //showRoomPos = showRoomRotate * showRoomPos;

        // Sky
        mesh = w.createMesh();
        Qtr3d::meshByStarsky(*mesh,1000,1000,Qt::white);
        w.createState(mesh);

        mesh = w.createMesh();
        Qtr3d::meshByStarsky(*mesh,1000,100,Qt::blue);
        w.createState(mesh);

        new Qtr3dOrbitCameraController(w.camera(),30,0.3,{0,2,5},{0,0,0});
    });

    w.show();

    return app.exec();
}
