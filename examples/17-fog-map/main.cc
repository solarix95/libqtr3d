#include <QApplication>
#include <QDebug>
#include <QSurfaceFormat>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dmesh.h>
#include <libqtr3d/qtr3dcameracycler.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dfactory.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w(Qtr3dWidget::MSAA4);
    w.setGeometry(50,50,500,500);


    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {
        w.bufferContext()->environment().setClearColor(QColor("#87ceeb"));
        w.bufferContext()->environment().setFogDistance(1500);

        auto *mesh = w.createMesh();

        QString hightmap  = ":/heightmap.png";
        QString colormap  = ":/texture.png";

        if (app.arguments().count() == 3) {
            hightmap = app.arguments()[1];
            colormap = app.arguments()[2];
        }

        // Variant 1: Colormap instead of Texture (faster, less memory, but lower resolution)
        // Qtr3d::meshByHighmap(*mesh,hightmap, colormap, {1,150,1});

        // Variant 2: Use real Texture:
        Qtr3d::meshByHighmap(*mesh,hightmap, QImage(colormap), {1,150,1});

        w.createState(mesh)->setLightingType(Qtr3d::NoLighting);

        new Qtr3dCameraCycler(w.camera(),50,0.05,{0,200,500},{0,0,0});
    });

    w.show();

    return app.exec();
}
