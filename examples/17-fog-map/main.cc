#include <QApplication>
#include <QDebug>
#include <QSurfaceFormat>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dmesh.h>
#include <libqtr3d/qtr3dcameracycler.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dmodelfactory.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w;
    w.setGeometry(50,50,500,500);


    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {
        w.bufferContext()->environment().setClearColor(QColor("#87ceeb"));
        w.bufferContext()->environment().setFogDistance(1500);

        auto *mesh = w.createMesh();

        Qtr3dModelFactory::meshByHighmap(*mesh,":/heightmap.png", ":/texture.jpg", {1,100,1});

        w.createState(mesh)->setLightingType(Qtr3d::NoLighting);

        new Qtr3dCameraCycler(w.camera(),30,0.1,{0,200,500},{0,0,0});
    });

    w.show();

    return app.exec();
}
