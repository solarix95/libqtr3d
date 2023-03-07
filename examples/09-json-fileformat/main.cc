#include <QApplication>
#include <QDebug>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dmesh.h>
#include <libqtr3d/qtr3dcameracycler.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dmodelfactory.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w;
    w.setGeometry(10,10,500,500);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {
        qDebug() << "OpenGL ready";

        auto *mesh = w.createMesh();
        Qtr3dModelFactory::meshByJson(*mesh,QString(":/demodata.json"));
        w.createState(mesh);

        // Sky
        mesh = w.createMesh();
        Qtr3dModelFactory::meshByStarsky(*mesh,1000,1000,Qt::white);
        w.createState(mesh);

        mesh = w.createMesh();
        Qtr3dModelFactory::meshByStarsky(*mesh,1000,100,Qt::blue);
        w.createState(mesh);

        new Qtr3dCameraCycler(w.camera(),30,0.3,{0,3,12},{0,0,0});
    });

    w.show();

    return app.exec();
}
