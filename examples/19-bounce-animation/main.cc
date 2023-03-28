#include <QApplication>
#include <QDebug>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dmesh.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dlightsource.h>
#include <libqtr3d/qtr3dfactory.h>
#include <libqtr3d/extras/qtr3dcameracycler.h>
#include <libqtr3d/physics/qtrphspace.h>
#include <libqtr3d/physics/qtr3dforcefield.h>
#include <libqtr3d/physics/qtr3dentity.h>
#include <libqtr3d/physics/qtr3dfpsloop.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w;
    w.setGeometry(50,50,500,500);
    w.setOptions(Qtr3dWidget::MSAA16);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {

        w.bufferContext()->environment().setClearColor(Qt::white);

        // Create the ball model
        auto *mesh = w.createMesh();
        Qtr3d::meshBySphere(*mesh,32,Qt::green);
        mesh->material().ambient().setStrength(0.3);
        mesh->material().diffuse().setStrength(0.0);
        mesh->material().specular().setStrength(0.7);
        auto *ball = w.createState(mesh);
        ball->setLightingType(Qtr3d::PhongLighting);
        ball->setPos({0,5,0});

        // Create the floor
        mesh = w.createMesh();
        mesh->startMesh(Qtr3d::Quad);
        mesh->setTexture(QImage(":/metal_texture_800.jpg"));
        mesh->addQuad({-10,0,10},{-10,0,-10},{10,0,-10}, {10,0,10},{1,1,0});
        mesh->endMesh();
        w.createState(mesh)->setLightingType(Qtr3d::PhongLighting);

        w.primaryLightSource()->setPos({30,30,0});
        w.primaryLightSource()->setAmbientStrength(0.1);
        w.camera()->lookAt({0,10,20},{0,0,0}, {0,1,0});
        new Qtr3dCameraCycler(w.camera(),50,0.1,{0,10,20},{0,0,0});

        w.bufferContext()->space().forceField()->setConstantForce({0,-0.0001,0});
        auto *ballEntity = new Qtr3dEntity(*ball);
        ballEntity->setPos({0,5,0});

        w.bufferContext()->space().append(ballEntity);
        w.bufferContext()->loop().setFps(50);
        QObject::connect(&w.bufferContext()->loop(),&Qtr3dFpsLoop::stepDone, ballEntity, [ballEntity]() {
            if (ballEntity->pos()[1] < ballEntity->collisionRadius()) {
                if (ballEntity->movement()[1] < 0)
                    ballEntity->setMovement(-ballEntity->movement());
            }
        });

    });

    w.show();

    return app.exec();
}
