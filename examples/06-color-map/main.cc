#include <QApplication>
#include <QDebug>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dvertexmesh.h>
#include <libqtr3d/qtr3dcameracycler.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dmodelfactory.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w;
    w.setGeometry(10,10,500,500);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {
        qDebug() << "OpenGL ready";

        Qtr3dVertexMesh *mesh;
        Qtr3dGeometryBufferState *state;

        // Sphere
        mesh = w.createVertexMesh();
        Qtr3dModelFactory::meshBySphere(*mesh,600,QImage(":/planet.jpg"));

        state = w.createBufferState(mesh);
        state->setFlat(false);
        state->setState({0,0,0},{-90,0,0},{2,2,2});

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
