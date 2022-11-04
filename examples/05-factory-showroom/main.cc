#include <QApplication>
#include <QDebug>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dvertexmesh.h>
#include <libqtr3d/qtr3dcameracycler.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dmodelfactory.h>
#include "solarsystem.h"

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w;
    w.setGeometry(10,10,500,500);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {
        qDebug() << "OpenGL ready";

        const int itemsCount = 4;
        QVector3D showRoomPos(0,0,-3);
        QMatrix4x4 showRoomRotate;
        showRoomRotate.rotate(360/itemsCount,{0,1,0});

        Qtr3dVertexMesh *mesh;
        Qtr3dGeometryBufferState *state;

        //mesh = w.createVertexMesh();
        //Qtr3dModelFactory::meshBySphere(*mesh,13,Qt::red);
        //w.createBufferState(mesh)->setFlat(false);

        mesh = w.createVertexMesh();
        Qtr3dModelFactory::meshByXyzAxis(*mesh);
        w.createBufferState(mesh)->setFlat(true);

        // Cycle
        mesh = w.createVertexMesh();
        Qtr3dModelFactory::meshByCycle(*mesh,13,Qt::red);
        state = w.createBufferState(mesh);
        state->setFlat(false);
        state->setState(showRoomPos,{0,0,45});
        showRoomPos = showRoomRotate * showRoomPos;

        // Sphere
        mesh = w.createVertexMesh();
        Qtr3dModelFactory::meshBySphere(*mesh,13,Qt::red);
        state = w.createBufferState(mesh);
        state->setFlat(false);
        state->setState(showRoomPos,{0,0,0});
        showRoomPos = showRoomRotate * showRoomPos;


        // Cylinder
        mesh = w.createVertexMesh();
        Qtr3dModelFactory::meshByCylinder(*mesh,13,false,false,Qt::red);
        state = w.createBufferState(mesh);
        state->setFlat(false);
        state->setState(showRoomPos,{0,0,0});
        showRoomPos = showRoomRotate * showRoomPos;

        // Solar System
        SolarSystem *solar = new SolarSystem(w.textures(),&w);
        w.createBufferState(solar)->setState(showRoomPos,{0,0,0},{0.1,0.1,0.1});
        showRoomPos = showRoomRotate * showRoomPos;

        // Sky
        mesh = w.createVertexMesh();
        Qtr3dModelFactory::meshByStarsky(*mesh,1000,1000,Qt::white);
        w.createBufferState(mesh);

        mesh = w.createVertexMesh();
        Qtr3dModelFactory::meshByStarsky(*mesh,1000,100,Qt::blue);
        w.createBufferState(mesh);

        new Qtr3dCameraCycler(w.camera(),30,0.3,{0,2,5},{0,0,0});
    });

    w.show();

    return app.exec();
}
