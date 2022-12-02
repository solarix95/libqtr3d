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
    w.setGeometry(50,50,500,500);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {

        auto *mesh = w.createVertexMesh()->startMesh(Qtr3dVertexMesh::Triangle, Qtr3dGeometryBuffer::CounterClockWise);

        mesh->addVertex({-2,-1,0},Qt::red);
        mesh->addVertex({0,1,0},Qt::green);
        mesh->addVertex({2,-1,0},Qt::blue);

        mesh->endMesh();

        w.createBufferState(mesh);

        w.camera()->lookAt({0,0,-5},{0,0,0}, {0,1,0});
    });

    w.show();

    return app.exec();
}
