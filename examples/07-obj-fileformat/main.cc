#include <QApplication>
#include <QDebug>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dmesh.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dfactory.h>
#include <libqtr3d/qtr3dlightsource.h>
#include <libqtr3d/extras/qtr3dorbitcameracontroller.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w(Qtr3dWidget::MSAA4);

    w.setGeometry(10,10,500,500);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {

        w.setDefaultLighting(Qtr3d::FlatLighting);

        Qtr3dModel *model;
        Qtr3dGeometryState *state;

        // OBJ-File
        model = w.createModel();
        model->setDefaultColor(Qt::red);

        // OBJ Model by https://github.com/nasa/NASA-3D-Resources/
        Qtr3d::modelByFile(*model,":/NASA3D-Z2-Spacesuit.obj");
        model->setFaceOrientation(Qtr3d::CounterClockWise);
        model->meshes()[0]->material().ambient().strength = { 0.1, 0.1, 0.1 };
        state = w.createState(model);
        state->setState({0,-1,0},{0,0,0},{1,1,1});

        // Sky
        auto *mesh = w.createMesh();
        Qtr3d::meshByStarsky(*mesh,1000,1000,Qt::white);
        w.createState(mesh, Qtr3d::NoLighting);

        mesh = w.createMesh();
        Qtr3d::meshByStarsky(*mesh,1000,100,Qt::blue);
        w.createState(mesh, Qtr3d::NoLighting);

        float dist = model->radius()*2;
        new Qtr3dOrbitCameraController(w.camera(),30,0.3,{0,dist,dist},{0,0,0});
        w.primaryLightSource()->setPos({0,0,dist});
    });

    w.show();

    return app.exec();
}
