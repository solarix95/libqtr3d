#include <QApplication>
#include <QDebug>
#include <QSurfaceFormat>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dmesh.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/extras/qtr3dorbitcameracontroller.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w;
    w.setGeometry(50,50,500,500);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {

        auto *buffer = w.createMesh();

        buffer->setTexture(QImage(":/texture.jpg"));
        buffer->startMesh(Qtr3d::Triangle, Qtr3d::CounterClockWise);

        buffer->addQuad({-1, 1,  1}, { 1, 1, 1}, { 1, 1,-1}, {-1, 1,-1});  // Top
        buffer->addQuad({-1, 1, -1}, { 1, 1,-1}, { 1,-1,-1}, {-1,-1,-1});  // Front

        buffer->addQuad({-1, 1, -1},{ 1, 1,-1},{ 1,-1,-1},{-1,-1,-1});     // Front

        buffer->addQuad({ 1,-1,  1}, { 1,-1,-1}, { 1, 1,-1}, { 1, 1, 1});  // Left
        buffer->addQuad({-1, 1,  1}, {-1, 1,-1}, {-1,-1,-1}, {-1,-1, 1});  // Right
        buffer->addQuad({-1,-1,  1}, { 1,-1, 1}, { 1, 1, 1}, {-1, 1, 1});  // Back
        buffer->addQuad({-1,-1, -1}, { 1,-1,-1}, { 1,-1, 1}, {-1,-1, 1});  // Bottom

        buffer->endMesh();

        w.createState(buffer)->setLightingType(Qtr3d::NoLighting);

        // lets show 2 more instances of the same shape:
        w.createState(buffer)->move({-3,0,0});
        w.createState(buffer)->move({ 3,0,0});

        new Qtr3dOrbitCameraController(w.camera(),30,{0.3,0.3,0.3},{0,0,-12},{0,0,0});
    });

    w.show();

    return app.exec();
}
