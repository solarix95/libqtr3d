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

        // green, top left triangle
        auto *mesh = w.createMesh()->startMesh(Qtr3d::Triangle, Qtr3d::ClockWise);
        mesh->setDefaultColor(Qt::green);
        mesh->addVertex({-2,-1,0});
        mesh->addVertex({0,1,0});
        mesh->addVertex({2,-1,0});
        mesh->endMesh();
        w.createState(mesh)->setPos({-2,2,-10});

        // colored, top right triangle
        mesh = w.createMesh()->startMesh(Qtr3d::Triangle, Qtr3d::ClockWise);
        mesh->setDefaultColor(Qt::green);
        mesh->addVertex({-2,-1,0},Qt::red);
        mesh->addVertex({0,1,0},Qt::green);
        mesh->addVertex({2,-1,0},Qt::blue);
        mesh->endMesh();
        w.createState(mesh)->setPos({2,2,-10});

        // colored, top right triangle
        mesh = w.createMesh()->startMesh(Qtr3d::Triangle, Qtr3d::ClockWise);
        mesh->setDefaultColor(Qt::green);
        mesh->addVertex({-2,-1,0},Qt::red);
        mesh->addVertex({0,1,0},Qt::green);
        mesh->addVertex({2,-1,0},Qt::blue);
        mesh->endMesh();
        w.createState(mesh)->setPos({2,2,-10});

        // Textured, bottom center triangle
        mesh = w.createMesh()->startMesh(Qtr3d::Triangle, Qtr3d::ClockWise);
        mesh->setTexture(QImage(":/texture.png"));
        mesh->addVertex({-2,-1,0},0,0);
        mesh->addVertex({0,  1,0},0.5,1);
        mesh->addVertex({2, -1,0},1,0);
        mesh->endMesh();
        w.createState(mesh)->setPos({0,0,-5});

        w.camera()->lookAt({0,0,0},{0,0,-10}, {0,1,0});
    });

    w.show();

    return app.exec();
}
