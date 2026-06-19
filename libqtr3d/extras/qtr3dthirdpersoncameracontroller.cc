#include <QTimer>
#include "qtr3dthirdpersoncameracontroller.h"
#include "../qtr3dcamera.h"
#include "../qtr3dgeometrystate.h"
#include "../qtr3dwidget.h"

Qtr3dThirdPersonCameraController::Qtr3dThirdPersonCameraController(Qtr3dWidget *parent, Qtr3dGeometryState *targetState, float distance, float height)
    : QObject(parent)
    , mCamera(parent ? parent->camera() : nullptr)
    , mTargetState(targetState)
    , mTargetDirection(0,0,-1)
    , mDistance(distance)
    , mHeight(height)
{
    Q_ASSERT(parent);
    Q_ASSERT(mCamera);
    Q_ASSERT(mTargetState);
    auto *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Qtr3dThirdPersonCameraController::updateCamera);
    timer->start(33);
    updateCamera();
}

void Qtr3dThirdPersonCameraController::setTargetDirection(const QVector3D &direction)
{
    if (!direction.isNull())
        mTargetDirection = direction.normalized();
}

void Qtr3dThirdPersonCameraController::updateCamera()
{
    const Qtr3dDblVector3D center = mTargetState->center();
    const QVector3D back = mTargetDirection.isNull() ? QVector3D(0,0,1) : -mTargetDirection.normalized();
    const Qtr3dDblVector3D pos = center + Qtr3dDblVector3D(back*mDistance + QVector3D(0,mHeight,0));
    mCamera->lookAt(pos, center + Qtr3dDblVector3D(0,mHeight*0.35f,0), QVector3D(0,1,0));
}
