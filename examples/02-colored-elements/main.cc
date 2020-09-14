#include <QApplication>
#include <QDebug>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dvertexmesh.h>
#include <libqtr3d/qtr3dcameracycler.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/debug/qtr3dfreecameracontroller.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w;
    w.setGeometry(10,10,500,500);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {
        qDebug() << "OpenGL ready";

        Qtr3dVertexMesh *mesh = w.createVertexMesh();

        mesh->startMesh(Qtr3dVertexMesh::Line);
        mesh->addVertex({0,0,0},Qt::white);
        mesh->addVertex({4,0,0},Qt::red);
        mesh->addVertex({0,0,0},Qt::white);
        mesh->addVertex({0,4,0},Qt::green);
        mesh->addVertex({0,0,0},Qt::white);
        mesh->addVertex({0,0,4},Qt::blue);
        mesh->endMesh();

        w.createBufferState(mesh);

        new Qtr3dCameraCycler(w.camera(),30,1,{0,1,1},{0,0,0});
        // w.camera()->lookAt({1,1,1}, {0,0,0},{0,1,0});
        // new Qtr3dFreeCameraController(&w);
    });

    w.show();

    return app.exec();
}
