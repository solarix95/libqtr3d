
#include <QKeyEvent>

#include "hawksim.h"
#include <libqtr3d/qtr3dmesh.h>
#include <libqtr3d/qtr3dmodel.h>
#include <libqtr3d/qtr3dtypes.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dassets.h>
#include <libqtr3d/qtr3dfactory.h>
#include <libqtr3d/extras/qtr3dfollowcamera.h>
#include <libqtr3d/physics/qtr3dabstractspace.h>
#include <libqtr3d/physics/qtr3dstandardentity.h>

//-------------------------------------------------------------------------------------------------
HawkSim::HawkSim()
 : mCamera(nullptr)
{
}

//-------------------------------------------------------------------------------------------------
void HawkSim::init(Qtr3dAssets *assets, Qtr3dCamera *camera)
{
    mCamera = camera;

    assets->environment().setClearColor(QColor("#87ceeb"));
    assets->environment().setFogDistance(1500);

    auto *mesh = assets->createMesh();

    Qtr3d::meshByChessboard(*mesh, 100, 1000, Qt::black, Qt::white);

    assets->createState(mesh)->setLightingType(Qtr3d::NoLighting);

    auto *hawk = assets->createModel();
    Qtr3d::modelByFile(*hawk,":GlobalHawk/GlobalHawkOBJ.obj");
    hawk->setFaceOrientation(Qtr3d::CounterClockWise);

    auto *hawkState = assets->createState(hawk);
    hawkState->setScale(0.1);

    hawkState->setLightingType(Qtr3d::PhongLighting);

    mHawk = new Qtr3dStandardEntity(hawkState,{0,0,0});
    mHawk->setLookAt({0,0,1});

    assets->space().append(mHawk);

    mCamera->lookAt({200,200,200},{0,0,0},{0,1,0});
    auto *followCam = new Qtr3dFollowCamera(mCamera->widget(),hawkState, 50,20);
    followCam->setTargetDirection(mHawk->lookAt());
    connect(mHawk, &Qtr3dStandardEntity::lookAtChanged,      followCam, &Qtr3dFollowCamera::setTargetDirection);
    connect(mHawk, &Qtr3dStandardEntity::orientationChanged, followCam, &Qtr3dFollowCamera::setTargetOrientation);
}

//-------------------------------------------------------------------------------------------------
bool HawkSim::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        auto *ke = dynamic_cast<QKeyEvent*>(event);
        Q_ASSERT(ke);

        if (ke->key() == Qt::Key_Up)    mHawk->relativeMove(1);
        if (ke->key() == Qt::Key_Down)  mHawk->relativeMove(-1);

        if (ke->key() == Qt::Key_W)     mHawk->relativePitch(-1);
        if (ke->key() == Qt::Key_S)     mHawk->relativePitch(+1);

        if (ke->key() == Qt::Key_Left)  mHawk->relativeStrafe(-1);
        if (ke->key() == Qt::Key_Right) mHawk->relativeStrafe(+1);

        if (ke->key() == Qt::Key_A)     mHawk->relativeTurn(+5);
        if (ke->key() == Qt::Key_D)     mHawk->relativeTurn(-5);

        if (ke->key() == Qt::Key_Q)     mHawk->relativeRoll(+5);
        if (ke->key() == Qt::Key_E)     mHawk->relativeRoll(-5);
    }

    return QObject::eventFilter(obj,event);
}
