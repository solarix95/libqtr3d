#include <QApplication>
#include <QDebug>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dmesh.h>
#include <libqtr3d/qtr3dcamera.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w;
    w.setGeometry(50,50,640,480);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {

        // Textured, bottom center triangle
        auto *mesh = w.createMesh()->startMesh(Qtr3d::Quad, Qtr3d::ClockWise);
        mesh->setTexture(QImage(":/texture.png"));
        mesh->addVertex({-1,-1,0},0,0);
        mesh->addVertex({-1,+1,0},1,0);
        mesh->addVertex({+1,+1,0},1,1);
        mesh->addVertex({+1,-1,0},0,1);
        mesh->endMesh();
        w.createState(mesh)->setPos({0,0,-5});

        w.camera()->lookAt({0,0,0},{0,0,-10}, {0,1,0});
    });

    w.show();

    return app.exec();
}
