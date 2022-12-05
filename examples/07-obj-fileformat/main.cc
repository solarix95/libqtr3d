#include <QApplication>
#include <QSurfaceFormat>
#include <QDebug>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dvertexmesh.h>
#include <libqtr3d/qtr3dcameracycler.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dmodelfactory.h>
#include <libqtr3d/qtr3dlightsource.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w(Qtr3dWidget::MSAA4);

    w.setGeometry(10,10,500,500);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {
        qDebug() << "OpenGL ready";
        w.setDefaultLighting(Qtr3d::FlatLighting);

        Qtr3dModel *model;
        Qtr3dGeometryBufferState *state;

        // OBJ-File
        model = w.createModel();
        model->setDefaultColor(Qt::red);

        // OBJ Model by https://github.com/nasa/NASA-3D-Resources/
        Qtr3dModelFactory::modelByFile(*model,":/NASA3D-Z2-Spacesuit.obj", *w.factory());
        model->setFaceOrientation(Qtr3dVertexMesh::CounterClockWise);
        model->buffer(0)->material().kAmbient = { 0.1, 0.1, 0.1 };
        state = w.createBufferState(model);
        state->setState({0,-1,0},{0,0,0},{1,1,1});

        // Sky
        auto *mesh = w.createVertexMesh();
        Qtr3dModelFactory::meshByStarsky(*mesh,1000,1000,Qt::white);
        w.createBufferState(mesh, Qtr3d::NoLighting);

        mesh = w.createVertexMesh();
        Qtr3dModelFactory::meshByStarsky(*mesh,1000,100,Qt::blue);
        w.createBufferState(mesh, Qtr3d::NoLighting);

        float dist = model->radius()*1.5;
        new Qtr3dCameraCycler(w.camera(),30,0.3,{0,dist,dist},{0,0,0});
        w.primaryLightSource()->setPos({0,0,dist});
    });

    w.show();

    return app.exec();
}
