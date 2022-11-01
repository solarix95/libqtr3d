#include <QApplication>
#include <QDebug>
#include <QSurfaceFormat>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dvertexmesh.h>
#include <libqtr3d/qtr3dcameracycler.h>
#include <libqtr3d/qtr3dcamera.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w;
    w.setGeometry(50,50,500,500);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {

        auto *mesh = w.createVertexMesh()->startMesh(Qtr3dGeometryBuffer::Quad, Qtr3dGeometryBuffer::CounterClockWise);

        // roof
        mesh->addVertex({-1,1, 1},Qt::red);
        mesh->addVertex({ 1,1, 1},Qt::green);
        mesh->addVertex({ 1,1,-1},Qt::blue);
        mesh->addVertex({-1,1,-1},Qt::white);

        // front
        mesh->addVertex({-1, 1, -1},Qt::white);
        mesh->addVertex({ 1, 1, -1},Qt::blue);
        mesh->addVertex({ 1,-1, -1},Qt::green);
        mesh->addVertex({-1,-1, -1},Qt::red);

        // left
        mesh->addVertex({ 1,-1,  1},Qt::red);
        mesh->addVertex({ 1,-1, -1},Qt::white);
        mesh->addVertex({ 1, 1, -1},Qt::blue);
        mesh->addVertex({ 1, 1,  1},Qt::green);

        // back
        mesh->addVertex({-1,-1,  1},Qt::white);
        mesh->addVertex({ 1,-1,  1},Qt::blue);
        mesh->addVertex({ 1, 1,  1},Qt::green);
        mesh->addVertex({-1, 1,  1},Qt::red);

        // right
        mesh->addVertex({ -1, 1,  1},Qt::green);
        mesh->addVertex({ -1, 1, -1},Qt::blue);
        mesh->addVertex({ -1,-1, -1},Qt::white);
        mesh->addVertex({ -1,-1,  1},Qt::red);

        // bottom
        mesh->addVertex({-1,-1,-1},Qt::white);
        mesh->addVertex({ 1,-1,-1},Qt::blue);
        mesh->addVertex({ 1,-1, 1},Qt::green);
        mesh->addVertex({-1,-1, 1},Qt::red);

        mesh->endMesh();

        w.createBufferState(mesh);

        new Qtr3dCameraCycler(w.camera(),30,{0.3,0.3,0.3},{0,0,-12},{0,0,0});
    });

    w.show();

    return app.exec();
}