#include <QApplication>
#include <QDebug>
#include <QSurfaceFormat>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dtexturedmesh.h>
#include <libqtr3d/qtr3dcameracycler.h>
#include <libqtr3d/qtr3dcamera.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w;
    w.setGeometry(50,50,500,500);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {

        auto *buffer = w.createTexturedMesh(":/texture.jpg");

        buffer->startMesh();

        buffer->addQuad({-1, 1,  1}, { 1, 1, 1}, { 1, 1,-1}, {-1, 1,-1});  // Top
        buffer->addQuad({-1, 1, -1}, { 1, 1,-1}, { 1,-1,-1}, {-1,-1,-1});  // Front

        buffer->addQuad({-1, 1, -1},{ 1, 1,-1},{ 1,-1,-1},{-1,-1,-1});     // Front

        buffer->addQuad({ 1,-1,  1}, { 1,-1,-1}, { 1, 1,-1}, { 1, 1, 1});  // Left
        buffer->addQuad({-1, 1,  1}, {-1, 1,-1}, {-1,-1,-1}, {-1,-1, 1});  // Right
        buffer->addQuad({-1,-1,  1}, { 1,-1, 1}, { 1, 1, 1}, {-1, 1, 1});  // Back
        buffer->addQuad({-1,-1, -1}, { 1,-1,-1}, { 1,-1, 1}, {-1,-1, 1});  // Bottom

        buffer->endMesh();

        w.createBufferState(buffer);

        // lets show 2 more instances of the same shape:
        w.createBufferState(buffer)->move({-3,0,0});
        w.createBufferState(buffer)->move({ 3,0,0});

        new Qtr3dCameraCycler(w.camera(),30,{0.3,0.3,0.3},{0,0,-12},{0,0,0});
    });

    w.show();

    return app.exec();
}
