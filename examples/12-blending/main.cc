#include <QApplication>
#include <QDebug>
#include <QSurfaceFormat>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dmesh.h>
#include <libqtr3d/qtr3dcameracycler.h>
#include <libqtr3d/qtr3dcamera.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w;
    w.setGeometry(50,50,500,500);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {

        // Create simple Cube
        auto *cube = w.createMesh();
        cube->setTexture(QImage(":/texture.jpg"));
        cube->startMesh(Qtr3d::Triangle, Qtr3d::CounterClockWise);

        cube->addQuad({-1, 1,  1}, { 1, 1, 1}, { 1, 1,-1}, {-1, 1,-1});  // Top
        cube->addQuad({-1, 1, -1}, { 1, 1,-1}, { 1,-1,-1}, {-1,-1,-1});  // Front

        cube->addQuad({-1, 1, -1},{ 1, 1,-1},{ 1,-1,-1},{-1,-1,-1});     // Front

        cube->addQuad({ 1,-1,  1}, { 1,-1,-1}, { 1, 1,-1}, { 1, 1, 1});  // Left
        cube->addQuad({-1, 1,  1}, {-1, 1,-1}, {-1,-1,-1}, {-1,-1, 1});  // Right
        cube->addQuad({-1,-1,  1}, { 1,-1, 1}, { 1, 1, 1}, {-1, 1, 1});  // Back
        cube->addQuad({-1,-1, -1}, { 1,-1,-1}, { 1,-1, 1}, {-1,-1, 1});  // Bottom

        cube->endMesh();
        w.createState(cube);

        // textured Window
        // Texture by https://learnopengl.com/Advanced-OpenGL/Blending
        auto *window = w.createMesh();
        window->setTexture(QImage(":/blending_transparent_window.png"));
        window->setBlendingEnabled(true);           // Enable Alpha Transparency for blending_transparent_window.png
        window->startMesh(Qtr3d::Triangle, Qtr3d::CounterClockWise);
        window->addQuad({-1, 1, 0},{ 1, 1,0},{ 1,-1,0},{-1,-1,0});     // Front
        window->endMesh();
        auto *windowState = w.createState(window);
        windowState->setPos({0,0,-1.3});

        // Simple Vertex Window
       auto *glas = w.createMesh();
       glas->setDefaultColor(QColor(255,0,0,127)); // 127 =~ 0.5 Opacity
       glas->setBlendingEnabled(true);             // Enable Alpha Transparency
       glas->startMesh(Qtr3d::Triangle, Qtr3d::CounterClockWise);
       glas->addQuad({0, 1,  1}, {0, 1,-1}, {0,-1,-1}, {0,-1, 1});  // Right
       glas->endMesh();

       auto *glasState = w.createState(glas);
       glasState->setPos({-1.3,0,0});

        new Qtr3dCameraCycler(w.camera(),30,{0.3,0.3,0.3},{0,0,-12},{0,0,0});
    });

    w.show();

    return app.exec();
}
