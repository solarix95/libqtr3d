
#include <QApplication>
#include <QDebug>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dmesh.h>
#include <libqtr3d/qtr3dfactory.h>
#include <libqtr3d/qtr3dlightsource.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/extras/qtr3dfreecameracontroller.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w;
    w.setGeometry(50,50,500,500);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {

        auto *mesh = w.createMesh()->startMesh(Qtr3d::Triangle, Qtr3d::ClockWise);

        // front
        mesh->addVertex({-1, 1,  1},QVector3D({-1, 1, 1}).normalized(),Qt::white);   // 0
        mesh->addVertex({ 1, 1,  1},QVector3D({ 1, 1, 1}).normalized(),Qt::blue);    // 1
        mesh->addVertex({ 1,-1,  1},QVector3D({ 1,-1, 1}).normalized(),Qt::green);   // 2
        mesh->addVertex({-1,-1,  1},QVector3D({-1,-1, 1}).normalized(),Qt::red);     // 3

        // back
        mesh->addVertex({-1, 1,  -1},QVector3D({-1, 1, -1}).normalized(),Qt::white); // 4
        mesh->addVertex({ 1, 1,  -1},QVector3D({ 1, 1, -1}).normalized(),Qt::blue);  // 5
        mesh->addVertex({ 1,-1,  -1},QVector3D({ 1,-1, -1}).normalized(),Qt::green); // 6
        mesh->addVertex({-1,-1,  -1},QVector3D({-1,-1, -1}).normalized(),Qt::red);   // 7

        // front
        mesh->addIndex(0);
        mesh->addIndex(1);
        mesh->addIndex(2);

        mesh->addIndex(0);
        mesh->addIndex(2);
        mesh->addIndex(3);

        // back
        mesh->addIndex(6);
        mesh->addIndex(5);
        mesh->addIndex(4);

        mesh->addIndex(7);
        mesh->addIndex(6);
        mesh->addIndex(4);


        // right
        mesh->addIndex(1);
        mesh->addIndex(5);
        mesh->addIndex(2);

        mesh->addIndex(5);
        mesh->addIndex(6);
        mesh->addIndex(2);

        // left
        mesh->addIndex(7);
        mesh->addIndex(4);
        mesh->addIndex(0);

        mesh->addIndex(0);
        mesh->addIndex(3);
        mesh->addIndex(7);

        // top
        mesh->addIndex(4);
        mesh->addIndex(5);
        mesh->addIndex(1);

        mesh->addIndex(0);
        mesh->addIndex(4);
        mesh->addIndex(1);


        // Bottom -> no bottom... to lacy for today

        mesh->endMesh();

        auto *state = w.createState(mesh, Qtr3d::FlatLighting);
        Q_UNUSED(state);

        auto *stateLight = w.createState(mesh, Qtr3d::NoLighting);
        stateLight->setState({0,2,0},{0,0,0},{0.1,0.1,0.1});

        // Debug-Mesh:
        auto *mesh2 = w.createMesh();
        Qtr3d::normalMeshByMesh(*mesh2, *mesh, 1, Qt::red);
        auto *state2 = w.createState(mesh2, Qtr3d::NoLighting);
        Q_UNUSED(state2);

        w.primaryLightSource()->setPos({0,5,0});
        w.primaryLightSource()->setAmbientStrength(0.1);
        w.camera()->lookAt({5,5, 12},{0,0,0},{0,1,0});
        new Qtr3dFreeCameraController(&w);
    });

    w.show();

    return app.exec();
}
