#include <QApplication>
#include <QDebug>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dmesh.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dlightsource.h>
#include <libqtr3d/qtr3dfactory.h>
#include <libqtr3d/extras/qtr3dorbitcameracontroller.h>
#include <libqtr3d/physics/qtr3dabstractspace.h>
#include <libqtr3d/physics/qtr3dforcefield.h>
#include <libqtr3d/physics/qtr3dstandardentity.h>
#include <libqtr3d/physics/qtr3dfpsloop.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w;
    w.setGeometry(50,50,500,500);
    w.setOptions(Qtr3dWidget::MSAA16);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {

        w.assets()->environment().setClearColor(Qt::white);
        w.setDefaultLighting(Qtr3d::PhongLighting);

        // Let's create the graphic models and states first:
        // *************************************************
        // Create the ball model
        auto *mesh = w.createMesh();
        Qtr3d::meshBySphere(*mesh,64,QImage(":/rubber_texture_400.jpg")); // by Webtreats: https://www.flickr.com/photos/webtreatsetc/5643615278/
        mesh->material().ambient().setStrength(0.3);
        mesh->material().diffuse().setStrength(0.0);
        mesh->material().specular().setStrength(0.7);
        auto *ball = w.createState(mesh);
        ball->setPos({0,5,0});

        // Create the floor
        mesh = w.createMesh();
        mesh->startMesh(Qtr3d::Quad);
        mesh->setTexture(QImage(":/metal_texture_800.jpg"));              // by Webtreats 8 Fabulous Free Metal Textures: https://www.flickr.com/photos/webtreatsetc/4324648760
        mesh->addQuad({-10,0,10},{-10,0,-10},{10,0,-10}, {10,0,10},QVector3D({1,1,0}));
        mesh->endMesh();
        w.createState(mesh);

        w.primaryLightSource()->setPos({30,30,0});
        w.primaryLightSource()->setAmbientStrength(0.1);
        new Qtr3dOrbitCameraController(w.camera(),50,0.1,{0,10,20},{0,0,0});

        // Now let's add physic animation:
        // *************************************************

        // Basic Physic + Animation Setup
        w.assets()->loop().setFps(50);
        w.assets()->space().forceField().setConstantForce({0,-0.00001,0}); // Simulate "Gravitation"

        // Create the "physical" representation of the ball:
        auto *ballEntity = new Qtr3dStandardEntity(ball,{0,5,0});
        w.assets()->space().append(ballEntity);
        ballEntity->setPos({0,5,0}); // the "enity" controls now the graphical "state" and updates the position

        // Simple physical constrains:
        QObject::connect(&w.assets()->loop(),&Qtr3dFpsLoop::stepDone, ballEntity, [ballEntity]() {

            // Floor: "Bounce-Reaction"
            if (ballEntity->pos()[1] < ballEntity->collisionRadius()) {
                ballEntity->setPos({0,ballEntity->collisionRadius(),0});
                if (ballEntity->movement()[1] < 0)
                    ballEntity->setMovement(0.9 * -ballEntity->movement()); // reverse and "dampering" the movement
            }

            // "Kick" at the end of the animation:
            if ((abs((ballEntity->pos().length() - ballEntity->collisionRadius())) < 0.01) && (ballEntity->movement().length() < 0.01)) {
                ballEntity->setMovement({0,0.5,0});
            }
        });

        QObject::connect(&w.assets()->loop(), &Qtr3dFpsLoop::stepDone, &w,[&]() { w.update(); });
    });

    w.show();

    return app.exec();
}
