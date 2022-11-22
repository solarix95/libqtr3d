#include <QApplication>
#include <QDebug>
#include <QSurfaceFormat>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dgeometrybufferfactory.h>
#include <libqtr3d/qtr3dvertexmesh.h>
#include <libqtr3d/qtr3dmodelfactory.h>
#include <libqtr3d/debug/qtr3dfreecameracontroller.h>
#include <libqtr3d/qtr3dlightsource.h>
#include <libqtr3d/qtr3dcamera.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w;
    w.setGeometry(50,50,500,500);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {

        auto *mesh = w.createVertexMesh()->startMesh(Qtr3dGeometryBuffer::Quad, Qtr3dGeometryBuffer::CounterClockWise);

        // roof
        mesh->addVertex({-1,1, 1},{0,1,0}, Qt::red);
        mesh->addVertex({ 1,1, 1},{0,1,0},Qt::green);
        mesh->addVertex({ 1,1,-1},{0,1,0},Qt::blue);
        mesh->addVertex({-1,1,-1},{0,1,0},Qt::white);

        // front
        mesh->addVertex({-1, 1, -1},{0,0,-1},Qt::white);
        mesh->addVertex({ 1, 1, -1},{0,0,-1},Qt::blue);
        mesh->addVertex({ 1,-1, -1},{0,0,-1},Qt::green);
        mesh->addVertex({-1,-1, -1},{0,0,-1},Qt::red);

        // left
        mesh->addVertex({ 1,-1,  1},{1,0,0},Qt::red);
        mesh->addVertex({ 1,-1, -1},{1,0,0},Qt::white);
        mesh->addVertex({ 1, 1, -1},{1,0,0},Qt::blue);
        mesh->addVertex({ 1, 1,  1},{1,0,0},Qt::green);

        // back
        mesh->addVertex({-1,-1,  1},{0,0,1},Qt::white);
        mesh->addVertex({ 1,-1,  1},{0,0,1},Qt::blue);
        mesh->addVertex({ 1, 1,  1},{0,0,1},Qt::green);
        mesh->addVertex({-1, 1,  1},{0,0,1},Qt::red);

        // right
        mesh->addVertex({ -1, 1,  1},{-1,0,0},Qt::green);
        mesh->addVertex({ -1, 1, -1},{-1,0,0},Qt::blue);
        mesh->addVertex({ -1,-1, -1},{-1,0,0},Qt::white);
        mesh->addVertex({ -1,-1,  1},{-1,0,0},Qt::red);

        // bottom
        mesh->addVertex({-1,-1,-1},{0,-1,0},Qt::white);
        mesh->addVertex({ 1,-1,-1},{0,-1,0},Qt::blue);
        mesh->addVertex({ 1,-1, 1},{0,-1,0},Qt::green);
        mesh->addVertex({-1,-1, 1},{0,-1,0},Qt::red);

        mesh->endMesh();

        auto *state = w.createBufferState(mesh);
        state->setLightingType(Qtr3d::FlatLighting);

        auto *stateLight = w.createBufferState(mesh);
        stateLight->setLightingType(Qtr3d::NoLighting);
        stateLight->setState({0,2,0},{0,0,0},{0.1,0.1,0.1});

        // Debug-Mesh:
        auto *mesh2 = w.createVertexMesh();

        Qtr3dModelFactory::normalMeshByMesh(*mesh2, *mesh, 1);
        auto *state2 = w.createBufferState(mesh2);
        state2->setLightingType(Qtr3d::NoLighting);

        w.primaryLightSource()->setPos({0,5,0});
        w.camera()->lookAt({5,5,-12},{0,0,0},{0,1,0});
        new Qtr3dFreeCameraController(&w);
    });

    w.show();

    return app.exec();
}
