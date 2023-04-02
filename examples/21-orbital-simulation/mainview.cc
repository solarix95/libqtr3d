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
#include "celestialbody.h"

MainView::MainView()
{
    connect(this, &Qtr3dWidget::initialized, this, [this]{

        bufferContext()->environment().setClearColor(Qt::black);
        // Sky
        auto *mesh = createMesh();
        Qtr3d::meshByStarsky(*mesh,1000,10000,Qt::white);
        createState(mesh, Qtr3d::NoLighting);

        setDefaultLighting(Qtr3d::PhongLighting);

        // Let's create the graphic models and states first:
        // *************************************************
        // the sun
        mesh = createMesh();
        Qtr3d::meshBySphere(*mesh,64,QImage(":/sun.jpg")); // by Webtreats: https://www.flickr.com/photos/webtreatsetc/5643615278/
        mesh->material() = Qtr3d::Material(1,0,0);
        auto *sun1 = createState(mesh);
        sun1->setScale(2.0);
        sun1->setRotation({90,0,0});


        mesh = createMesh();
        Qtr3d::meshBySphere(*mesh,64,QImage(":/Alpine.png"));
        mesh->material() = Qtr3d::Material(0,0.8,0);
        auto *planet1 = createState(mesh);

        mesh = createMesh();
        Qtr3d::meshBySphere(*mesh,64,QImage(":/Tropical.png"));
        mesh->material() = Qtr3d::Material(0,0.8,0);
        auto *planet2 = createState(mesh);

        mesh = createMesh();
        Qtr3d::meshBySphere(*mesh,64,QImage(":/Swamp.png"));
        mesh->material() = Qtr3d::Material(0,1,0);
        auto *planet3 = createState(mesh);
        planet3->setScale(0.3);

        mesh = createMesh();
        Qtr3d::meshBySphere(*mesh,64,QImage(":/Savannah.png"));
        mesh->material() = Qtr3d::Material(0,1,0);
        auto *planet4 = createState(mesh);
        planet4->setScale(2);

        primaryLightSource()->setPos({0,0,0});
        primaryLightSource()->setAmbientStrength(0.1);

        camera()->setPos(0,0,-50);

        // Now let's add physic animation:
        // *************************************************
        bufferContext()->loop().setFps(50);
        bufferContext()->space().append(new CelestialBody(*sun1,0.1,{0,0,0},{0,0,0}));

        bufferContext()->space().append(new CelestialBody(*planet1,0.0001,{20,0,0},{0,0,-0.3}));
        bufferContext()->space().append(new CelestialBody(*planet2,0.0001,{30,0,0},{0,0,-0.25}));
        bufferContext()->space().append(new CelestialBody(*planet3,0.0001,{5,0,0},{0,0,-0.7}));
        bufferContext()->space().append(new CelestialBody(*planet4,0.0001,{50,20,0},{0,0,-0.2}));

        connect(&bufferContext()->loop(), &Qtr3dFpsLoop::stepDone, this,[this]() { update(); });

        new Qtr3dEgoCameraController(this);
    });

}
