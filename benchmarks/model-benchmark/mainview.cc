#include <QElapsedTimer>
#include <QPainter>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dmesh.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dlightsource.h>
#include <libqtr3d/qtr3dfactory.h>
#include <libqtr3d/extras/qtr3dcameracycler.h>
#include <libqtr3d/physics/qtr3dabstractspace.h>
#include <libqtr3d/physics/qtr3dforcefield.h>
#include <libqtr3d/physics/qtr3dstandardentity.h>
#include <libqtr3d/physics/qtr3dfpsloop.h>
#include "mainview.h"

MainView::MainView(QWidget *parent)
 : Qtr3dWidget(parent)
 , mStateCount(0)
 , mCpuLoad(0)
{
    connect(this, &Qtr3dWidget::initialized, this, [this]{

        bufferContext()->environment().setClearColor(Qt::black);

        auto *iss = createModel();
        Qtr3d::modelByFile(*iss,":/ISS_stationary.glb");

        QTimer *t = new QTimer(this);
        connect(t, &QTimer::timeout, this, [this,iss]() {
            float alpha = ((qrand() % 1000)/1000.0)*360;
            QMatrix4x4 m;
            m.rotate(alpha,{0,1,0});
            QVector3D v = {0.0,float(-5 + (qrand()%1000)/100.0) ,float(-5 - (qrand()%10000)/100.0)};
            v = m*v;
            auto *state = createState(iss,Qtr3d::NoLighting);
            state->setScale(1/iss->radius());
            state->setPos(v);
            state->setRotation({0,alpha,0});
            mStateCount++;
        });
        t->start(100);

        // Now let's add physic animation:
        // *************************************************
        bufferContext()->loop().setFps(50);

        connect(&bufferContext()->loop(), &Qtr3dFpsLoop::stepDone, this,[this]() { update(); });

        new Qtr3dCameraCycler(camera(),50,0.1,{0,0,2},{0,0,0});
    });

}

MainView::~MainView()
{
}

void MainView::paint3D()
{
    static int fpsCount = 0;
    QElapsedTimer t;
    t.start();
    Qtr3dWidget::paint3D();

    mElapsedTimes << t.elapsed();

    fpsCount++;

    if (fpsCount < 50)
        return;

    float mean = 0;
    for (auto t: mElapsedTimes)
        mean += t;
    mean = mean/mElapsedTimes.count();
    mCpuLoad = 100*(mean/bufferContext()->loop().interval());
    mElapsedTimes.clear();
    fpsCount = 0;
    emit statisticsChanged();
}

void MainView::paint2D()
{
    if (!mFpsTime.isValid()) {
        mFpsTime.start();
        return;
    }
    //p.drawText(0,0,width(),height(),Qt::AlignTop | Qt::AlignHCenter,QString("gugusli"));



    mFpsTime.start();
}
