#include <QApplication>
#include <QSurfaceFormat>
#include <QDebug>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dvertexmesh.h>
#include <libqtr3d/qtr3dcameracycler.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dmodelfactory.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w(Qtr3dWidget::MSAA16);

    w.setGeometry(10,10,500,500);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {
        qDebug() << "OpenGL ready";

        Qtr3dVertexMesh *mesh;
        Qtr3dGeometryBufferState *state;

        // Sphere
        auto *sphere = w.createVertexMesh();
        Qtr3dModelFactory::meshBySphere(*sphere,600,QImage(":/planet.jpg"));

        state = w.createBufferState(sphere);
        state->setFlat(false);
        state->setState({0,0,0},{-90,0,0},{1,1,1});

        // Sky
        mesh = w.createVertexMesh();
        Qtr3dModelFactory::meshByStarsky(*mesh,1000,1000,Qt::white);
        w.createBufferState(mesh)->setFlat(true);

        mesh = w.createVertexMesh();
        Qtr3dModelFactory::meshByStarsky(*mesh,1000,100,Qt::blue);
        w.createBufferState(mesh);

        // new Qtr3dCameraCycler(w.camera(),30,0.3,{0.0f,(float)(1.5*sphere->radius()),0.0f},{0,0,0});
        qDebug() << sphere->radius();
        new Qtr3dCameraCycler(w.camera(),30,0.3,{0,0,3*sphere->radius()},{0,0,0});
    });

    w.show();

    return app.exec();
}
