#include <QApplication>
#include <QDebug>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dtexturedquad.h>
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

        Qtr3dTexturedQuad *buffer = w.createTexturedQuad(":/lba.jpg");

        buffer->startMesh();

        buffer->addQuad({1,1,0},
                        {-1,1,0},
                        {-1,-1,0},
                        {1,-1,0});
        buffer->calculateNormals();
        buffer->endMesh();


        bool isFlat = false;
        w.createBufferState(buffer,isFlat)->move({0, 0, -2},  {0,0,0});     // front
        w.createBufferState(buffer,isFlat)->move({0, 1, -3},  {-90,0,0});    // top
        w.createBufferState(buffer,isFlat)->move({0,-1, -3},  { 90,0,0});    // bottom
        w.createBufferState(buffer,isFlat)->move({1, 0, -3},  {0, 90,  0});    // right
        w.createBufferState(buffer,isFlat)->move({-1,0, -3},  {0,-90,  0});    // left

        // w.camera()->setPosition(0,0,0);
        new Qtr3dCameraCycler(w.camera(),30,1,{0,2,0},{0,0,-3});
        // w.camera()->lookAt({0,0,0}, {0,0,-2},{0,1,0});
        // new Qtr3dFreeCameraController(&w);
    });

    w.show();

    return app.exec();
}
