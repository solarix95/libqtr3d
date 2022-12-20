#include <QApplication>
#include <QDebug>
#include <QSurfaceFormat>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dtexturedmesh.h>
#include <libqtr3d/qtr3dvertexmesh.h>
#include <libqtr3d/qtr3dcameracycler.h>
#include <libqtr3d/qtr3dcamera.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w;
    w.setGeometry(50,50,500,500);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {

        // Create simple Cube
        auto *cube = w.createTexturedMesh(":/texture.jpg");

        cube->setFaceOrientation(Qtr3dGeometryBuffer::CounterClockWise);
        cube->startMesh();

        cube->addQuad({-1, 1,  1}, { 1, 1, 1}, { 1, 1,-1}, {-1, 1,-1});  // Top
        cube->addQuad({-1, 1, -1}, { 1, 1,-1}, { 1,-1,-1}, {-1,-1,-1});  // Front

        cube->addQuad({-1, 1, -1},{ 1, 1,-1},{ 1,-1,-1},{-1,-1,-1});     // Front

        cube->addQuad({ 1,-1,  1}, { 1,-1,-1}, { 1, 1,-1}, { 1, 1, 1});  // Left
        cube->addQuad({-1, 1,  1}, {-1, 1,-1}, {-1,-1,-1}, {-1,-1, 1});  // Right
        cube->addQuad({-1,-1,  1}, { 1,-1, 1}, { 1, 1, 1}, {-1, 1, 1});  // Back
        cube->addQuad({-1,-1, -1}, { 1,-1,-1}, { 1,-1, 1}, {-1,-1, 1});  // Bottom

        cube->endMesh();
        w.createBufferState(cube);

        // textured Window
        // Texture by https://learnopengl.com/Advanced-OpenGL/Blending
        auto *window = w.createTexturedMesh(":/blending_transparent_window.png");
        window->setFaceOrientation(Qtr3dGeometryBuffer::CounterClockWise);
        window->setBlendingEnabled(true);           // Enable Alpha Transparency for blending_transparent_window.png
        window->startMesh();
        window->addQuad({-1, 1, 0},{ 1, 1,0},{ 1,-1,0},{-1,-1,0});     // Front
        window->endMesh();
        auto *windowState = w.createBufferState(window);
        windowState->setPos({0,0,-1.3});

        // Simple Vertex Window
       auto *glas = w.createVertexMesh();
       glas->setDefaultColor(QColor(255,0,0,127)); // 127 =~ 0.5 Opacity
       glas->setBlendingEnabled(true);             // Enable Alpha Transparency
       glas->startMesh(Qtr3dVertexMesh::Triangle, Qtr3dGeometryBuffer::CounterClockWise);
       glas->addQuad({0, 1,  1}, {0, 1,-1}, {0,-1,-1}, {0,-1, 1});  // Right
       glas->endMesh();

       auto *glasState = w.createBufferState(glas);
       glasState->setPos({-1.3,0,0});

        new Qtr3dCameraCycler(w.camera(),30,{0.3,0.3,0.3},{0,0,-12},{0,0,0});
    });

    w.show();

    return app.exec();
}
