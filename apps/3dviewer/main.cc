#include <QApplication>
#include "viewerform.h"

#include <QSurfaceFormat>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    ViewerForm form;
    form.show();
    /*
    Qtr3dWidget w(Qtr3dWidget::MSAA16);

    w.setGeometry(10,10,500,500);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {

        // STL-File
        auto *stlMesh = w.createVertexMesh();
        stlMesh->setDefaultColor(Qt::white);

        // STL Model by https://en.wikipedia.org/wiki/STL_(file_format)
        Qtr3dModelFactory::meshByFile(*stlMesh,":/Sphericon.stl");

        Qtr3dGeometryBufferState *state;
        state = w.createBufferState(stlMesh);
        state->setFlat(false);
        state->setState({0,-1,0},{0,0,0},{1,1,1});

        // Sky
        Qtr3dVertexMesh *mesh;
        mesh = w.createVertexMesh();
        Qtr3dModelFactory::meshByStarsky(*mesh,stlMesh->radius()*10,1000,Qt::white);
        w.createBufferState(mesh);

        new Qtr3dCameraCycler(w.camera(),30,0.3,{0,(float)(stlMesh->radius()),(float)(2*stlMesh->radius())},stlMesh->center());
    });

    w.show();

    */
    return app.exec();
}
