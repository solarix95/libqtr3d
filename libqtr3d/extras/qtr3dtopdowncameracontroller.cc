#include <QTimer>
#include "qtr3dtopdowncameracontroller.h"
#include "../qtr3dcamera.h"
#include "../qtr3dgeometrystate.h"
#include "../qtr3dwidget.h"

Qtr3dTopDownCameraController::Qtr3dTopDownCameraController(Qtr3dWidget *parent, const QVector3D &center, float height)
    : Qtr3dWidgetInputController(parent)
    , mCamera(parent ? parent->camera() : nullptr)
    , mTargetState(nullptr)
    , mCenter(center)
    , mHeight(height)
    , mPanStep(height*0.04f)
{
    Q_ASSERT(parent);
    parent->installEventFilter(this);
    updateCamera();
}

Qtr3dTopDownCameraController::Qtr3dTopDownCameraController(Qtr3dWidget *parent, Qtr3dGeometryState *targetState, float height)
    : Qtr3dWidgetInputController(parent)
    , mCamera(parent ? parent->camera() : nullptr)
    , mTargetState(targetState)
    , mCenter(0,0,0)
    , mHeight(height)
    , mPanStep(height*0.04f)
{
    Q_ASSERT(parent);
    parent->installEventFilter(this);
    auto *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Qtr3dTopDownCameraController::updateCamera);
    timer->start(33);
    updateCamera();
}

void Qtr3dTopDownCameraController::parentKeyPress(QKeyEvent *e)
{
    if (mTargetState)
        return;
    if (e->key() == Qt::Key_W || e->key() == Qt::Key_Up)
        mCenter.setZ(mCenter.z() - mPanStep);
    if (e->key() == Qt::Key_S || e->key() == Qt::Key_Down)
        mCenter.setZ(mCenter.z() + mPanStep);
    if (e->key() == Qt::Key_A || e->key() == Qt::Key_Left)
        mCenter.setX(mCenter.x() - mPanStep);
    if (e->key() == Qt::Key_D || e->key() == Qt::Key_Right)
        mCenter.setX(mCenter.x() + mPanStep);
    updateCamera();
}

void Qtr3dTopDownCameraController::parentMouseWheel(float delta)
{
    mHeight *= delta > 0 ? 0.9f : 1.1f;
    if (mHeight < 1.0f)
        mHeight = 1.0f;
    mPanStep = mHeight*0.04f;
    updateCamera();
}

void Qtr3dTopDownCameraController::parentDraggingRel(QPointF delta)
{
    if (mTargetState)
        return;
    mCenter += QVector3D(float(-delta.x()*mHeight), 0, float(-delta.y()*mHeight));
    updateCamera();
}

void Qtr3dTopDownCameraController::updateCamera()
{
    QVector3D center = mTargetState ? mTargetState->center().toFloat() : mCenter;
    mCamera->lookAt(QVector3D(center.x(), center.y() + mHeight, center.z()), center, QVector3D(0,0,-1));
}
