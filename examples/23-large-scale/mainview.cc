#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dmesh.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dlightsource.h>
#include <libqtr3d/qtr3dfactory.h>
#include <libqtr3d/extras/qtr3degocameracontroller.h>
#include <libqtr3d/physics/qtr3dabstractspace.h>
#include <libqtr3d/physics/qtr3dforcefield.h>
#include <libqtr3d/physics/qtr3dstandardentity.h>
#include <libqtr3d/physics/qtr3dfpsloop.h>
#include "mainview.h"


MainView::MainView()
 : Qtr3dWidget((Options)(MSAA4|OriginRebasing))
{
    connect(this, &Qtr3dWidget::initialized, this, [this]{

        assets()->environment().setClearColor(Qt::black);

        setDefaultLighting(Qtr3d::NoLighting);

        // Let's create the graphic models and states first:
        // *************************************************

        // ISS-Model, Radius 1 OGL =100m
        auto *mesh = createMesh();
        Qtr3d::meshBySphere(*mesh,640,640, 1, true, QImage(":/missing.png"));
        mesh->setRenderOption(Qtr3d::ZOrderOption);
        auto *planet1 = createState(mesh);

        // Earth-Model, Radius 1 OGL =100m -> 6’371km
        mesh = createMesh();
        Qtr3d::meshBySphere(*mesh,512,1024,63710, true, QImage(":/1k_earth_daymap.jpg"));

        // mesh->setRenderOption(Qtr3dGeometry::NoZBufferOption);
        mesh->setRenderOption(Qtr3d::ZOrderOption);
        auto *planet2 = createState(mesh);
        planet2->setPos({0.0,0.0,63710 + 4000.0 /* ISS distance: 400km */ });


        // Sun-Model, Radius 1 OGL =100m -> 696’340km
        mesh = createMesh();
        Qtr3d::meshBySphere(*mesh,640,640,6963400, true, QImage(":/missing.png"));
        // mesh->setRenderOption(Qtr3dGeometry::NoZBufferOption);
        mesh->setRenderOption(Qtr3d::ZOrderOption);
        auto *planet3 = createState(mesh);
        planet3->setPos({500000000.0 /* shift.. otherwise it's behind the "earth" */,0.0,1495978700.0}); // 149.597.870km


        // Skybox
        mesh = createMesh();
        Qtr3d::meshBySphere(*mesh,640,640,100000, false, QImage(":/skybox.jpg"));
        mesh->setRenderOption(Qtr3d::BackgroundOption);
        auto *skybox = createState(mesh);

        camera()->lookAt({2,0,-5},{2,0,0},{0,1,0});
        camera()->setFov(50,0.001,1000000);

        // Very important: move the skybox always to cameras position:
        connect(camera(), &Qtr3dCamera::positionChanged, skybox, [this,skybox]() {
            skybox->setPos(camera()->pos());
        });

        // Now let's add physic animation:
        assets()->loop().setFps(50);

        connect(&assets()->loop(),&Qtr3dFpsLoop::stepDone, this, [this]() {
            static Qtr3dDblVector3D step = Qtr3dDblVector3D{0,0,1};
            static int direction = 1;
            if (direction > 0)
                step =  step + 0.01*step;
            camera()->setPos(camera()->pos() - step);
            update();

            if (camera()->pos().length() > 4.0e+06) {
                direction = -1;
                step = Qtr3dDblVector3D{0,0,-100000};
            }

            if (camera()->pos().z >= 0) {
                direction = 1;
                camera()->setPos({2,0,-5});
                step = Qtr3dDblVector3D{0,0,1};
            }
        });
    });

}
