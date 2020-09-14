#include <QApplication>
#include <QDebug>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dtexturedquad.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w;
    w.setGeometry(10,10,1000,1000);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {
        qDebug() << "OpenGL ready";

        Qtr3dGeometryBuffer *buffer = w.createTexturedQuad(QImage(":/lba.jpg"));

        w.createBufferState(buffer)->move({0,0,-5},  {0,0,0});     // front
        w.createBufferState(buffer)->move({0,2,-5},  {-45,0,0});   // top
        w.createBufferState(buffer)->move({0,-2,-5}, {45,0,0});    // bottom
        w.createBufferState(buffer)->move({2,0,-5},  {0,45,0});    // right
        w.createBufferState(buffer)->move({-2,0,-5}, {0,-45,0});   // left


    });

    w.show();

    return app.exec();
}
