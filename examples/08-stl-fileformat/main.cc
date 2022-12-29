#include <QApplication>
#include <QSurfaceFormat>
#include <QDebug>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dmesh.h>
#include <libqtr3d/qtr3dcameracycler.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dmodelfactory.h>
#include <libqtr3d/qtr3dlightsource.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w(Qtr3dWidget::MSAA16);

    w.setGeometry(10,10,500,500);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {

        // STL-File
        auto *stlMesh = w.createModel();
        stlMesh->setDefaultColor(Qt::white);

        // STL Model by https://en.wikipedia.org/wiki/STL_(file_format)
        Qtr3dModelFactory::modelByFile(*stlMesh,":/Utah_teapot.stl");

        Qtr3dGeometryBufferState *state;
        state = w.createState(stlMesh);
        state->setState({0,-1,0},{0,0,0},{1,1,1});
        state->setLightingType(Qtr3d::FlatLighting);

        // Sky
        Qtr3dMesh *mesh = w.createMesh();
        Qtr3dModelFactory::meshByStarsky(*mesh,stlMesh->radius()*10,1000,Qt::white);
        w.createState(mesh);

        w.primaryLightSource()->setPos({100,100,0});
        w.primaryLightSource()->setAmbientStrength(0.3);
        new Qtr3dCameraCycler(w.camera(),30,0.3,{0,(float)(stlMesh->radius()),(float)(2*stlMesh->radius())},stlMesh->center());
    });

    w.show();

    return app.exec();
}