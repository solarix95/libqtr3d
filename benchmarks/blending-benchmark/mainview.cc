#include <QTimer>
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

MainView::MainView()
 : mStateCount(0)
{
    connect(this, &Qtr3dWidget::initialized, this, [this]{

        bufferContext()->environment().setClearColor(Qt::black);

        // Sky
        auto *mesh = createMesh();
        Qtr3d::meshByStarsky(*mesh,1000,10000,Qt::white);
        createState(mesh, Qtr3d::NoLighting);

        auto *redModel = createMesh();
        Qtr3d::meshByText(*redModel,"R",QFont("Arial",200),QColor("#99FF0000"));
        redModel->setBlendingEnabled(true);
        auto *state = createState(redModel);
        state->setPos({0,0,-5});

        auto *greenModel = createMesh();
        Qtr3d::meshByText(*greenModel,"G",QFont("Arial",200),QColor("#9900FF00"));
        greenModel->setBlendingEnabled(true);
        state->setPos({0,0,-7});

        auto *blueModel = createMesh();
        Qtr3d::meshByText(*blueModel,"B",QFont("Arial",200),QColor("#990000FF"));
        blueModel->setBlendingEnabled(true);

        QTimer *t = new QTimer(this);
        connect(t, &QTimer::timeout, this, [this,redModel,greenModel,blueModel]() {
            QList<Qtr3dMesh*> meshes = QList<Qtr3dMesh*>() << redModel << greenModel << blueModel;
            float alpha = ((qrand() % 1000)/1000.0)*360;
            QMatrix4x4 m;
            m.rotate(alpha,{0,1,0});
            QVector3D v = {0,-5 + (qrand()%100)/10.0 ,-5 - (qrand()%1000)/10.0};
            v = m*v;
            auto *state = createState(meshes[qrand()%meshes.count()]);
            state->setPos(v);
            state->setRotation({0,alpha,0});
            mStateCount++;
        });
        t->start(1);

        // Now let's add physic animation:
        // *************************************************
        bufferContext()->loop().setFps(50);

        connect(&bufferContext()->loop(), &Qtr3dFpsLoop::stepDone, this,[this]() { update(); });

        new Qtr3dCameraCycler(camera(),50,0.1,{0,0,2},{0,0,0});
    });

}

void MainView::paint3D()
{
    static int fpsCount = 0;
    QTime t;
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
    float load = 100*(mean/bufferContext()->loop().interval());
    qDebug() << QString("Rendertime: %1[ms]").arg(mean,0,'f',3) << mStateCount << load;

    mElapsedTimes.clear();
    fpsCount = 0;
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
