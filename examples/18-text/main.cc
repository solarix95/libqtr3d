#include <QApplication>
#include <QDebug>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dmesh.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dfactory.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w;
    w.setGeometry(50,50,500,500);
    w.setOptions(Qtr3dWidget::MSAA16);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {

        // Create the Text
        auto *mesh = w.createMesh();
        Qtr3d::meshByText(*mesh,"Cuter 3D",QFont("Verdana",100),Qt::green);
        w.createState(mesh);

        // Create the floor to demonstrate transparency
        mesh = w.createMesh();
        mesh->startMesh(Qtr3d::Triangle);
        mesh->setDefaultColor(Qt::gray);
        mesh->addQuad({-3,-1,3},{-3,-1,-3},{+3,-1,-3},{3,-1,3},QVector3D({0,1,0}));
        mesh->endMesh();
        w.createState(mesh);

        w.camera()->lookAt({0,2,7},{0,0,0}, {0,1,0});
    });

    w.show();

    return app.exec();
}
