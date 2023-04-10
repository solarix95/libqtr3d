
#include <cmath>
#include <QDebug>
#include <QMouseEvent>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dmesh.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dlightsource.h>
#include <libqtr3d/qtr3dfactory.h>
#include <libqtr3d/physics/qtr3dfpsloop.h>

#include "mainwindow.h"

//-------------------------------------------------------------------------------------------------
MainWindow::MainWindow()
    : Qtr3dWidget()
    , mState(nullptr)
{
    setMouseTracking(true);

    QObject::connect(this, &Qtr3dWidget::initialized, [&]() {

        assets()->environment().setClearColor(Qt::black);
        camera()->setFov(20);

        // Create the floor
        auto *mesh = createMesh();
        Qtr3d::meshByTexture(*mesh,QImage(":/medfighter.png"), 10);
        mState = createState(mesh);

        mesh = createMesh();
        Qtr3d::meshByTexture(*mesh,QImage(":/385447-wallpaper.jpg"), 200);
        createState(mesh)->setPos({0,-1,0}); // let's place the background at position "-1", behind the starship...

        camera()->lookAt({0,200,0},{0,0,0},{0,0,-1});
    });
}

//-------------------------------------------------------------------------------------------------
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    Qtr3dWidget::mouseMoveEvent(event);
    if (!mState)
        return;

    QPointF    direction2d = event->pos() - QPointF(width()/2, height()/2);
    QPointF    reference(0,-1);

    float dot  = direction2d.x()*reference.x() + direction2d.y()*reference.y();
    float det  = direction2d.x()*reference.y() - direction2d.y()*reference.x();
    float angle= std::atan2(det,dot);
    angle = 360*(angle/float(2*3.1415));

    // rotate complete scene:
    // QVector3D  direction3d = {-direction2d.x(), 0, direction2d.y()};
    // camera()->lookAt({0,200,0},{0,0,0},direction3d);

    // rotate starship only
    mState->setRotation({0,angle,0});
    update();
}
