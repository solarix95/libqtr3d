#include <QApplication>
#include <QDebug>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dmesh.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dfactory.h>
#include <libqtr3d/extras/qtr3dcameracycler.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w;
    w.setGeometry(10,10,500,500);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {

        auto *mesh = w.createMesh();
        Qtr3d::meshByJson(*mesh,QString(":/demodata.json")); // Yea I know.. more feature needed!
        w.createState(mesh);

        // Sky
        mesh = w.createMesh();
        Qtr3d::meshByStarsky(*mesh,1000,1000,Qt::white);
        w.createState(mesh);

        mesh = w.createMesh();
        Qtr3d::meshByStarsky(*mesh,1000,100,Qt::blue);
        w.createState(mesh);

        new Qtr3dCameraCycler(w.camera(),30,0.3,{0,3,12},{0,0,0});
    });

    w.show();

    return app.exec();
}
