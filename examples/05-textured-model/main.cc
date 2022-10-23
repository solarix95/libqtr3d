#include <QApplication>
#include <QDebug>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dtexturedquad.h>
#include <libqtr3d/qtr3dvertexmesh.h>
#include "libqtr3d/qtr3dcameracycler.h"
#include "libqtr3d/qtr3dmodelfactory.h"

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w;
    w.setGeometry(10,10,1000,1000);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {
        qDebug() << "OpenGL ready";

        Qtr3dVertexMesh *mesh = w.createVertexMesh();
        // Qtr3dModelFactory::meshBySphere(*mesh,200,QImage(":/colormap.png"));
        Qtr3dModelFactory::meshBySphere(*mesh,200,QImage(":/planet.jpg"));

        Qtr3dGeometryBufferState *state = w.createBufferState(mesh);
        state->setFlat(false);
        state->setState({0,-300,0},{-90,0,0},{200,200,200});

        // Sky
        mesh = w.createVertexMesh();
        Qtr3dModelFactory::meshByStarsky(*mesh,1000,1000,Qt::white);
        state = w.createBufferState(mesh);
        state->setFlat(true);
        state->setState({0,0,0},{0,0,0},{500,500,500});

        Qtr3dTexturedQuad *buffer = w.createTexturedQuad();

        Qtr3dModelFactory::quadsByJson(*buffer,QString(":/demodata.json"));
        buffer->calculateNormals();
        state = w.createBufferState(buffer);
        state->setFlat(false);
        state->setState({0,0,0},{0,0,0},{0.1,0.1,0.1});


        new Qtr3dCameraCycler(w.camera(),30,0.1,{0,5,3},{0,0,0});
    });

    w.show();

    return app.exec();
}
