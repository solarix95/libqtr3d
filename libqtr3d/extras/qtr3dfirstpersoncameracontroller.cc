#include "qtr3dfirstpersoncameracontroller.h"
#include <QMatrix4x4>
#include "../qtr3dcamera.h"
#include "../qtr3dwidget.h"

Qtr3dFirstPersonCameraController::Qtr3dFirstPersonCameraController(Qtr3dWidget *parent, float moveStep)
    : Qtr3dWidgetInputController(parent)
    , mCamera(parent ? parent->camera() : nullptr)
    , mMoveStep(moveStep)
{
    Q_ASSERT(parent);
    parent->installEventFilter(this);
    Q_ASSERT(mCamera);
}

QVector3D Qtr3dFirstPersonCameraController::forward() const
{
    return (mCamera->lookAtCenter() - mCamera->pos()).toFloat().normalized();
}

QVector3D Qtr3dFirstPersonCameraController::right() const
{
    return QVector3D::crossProduct(forward(), QVector3D(0,1,0)).normalized();
}

void Qtr3dFirstPersonCameraController::moveCamera(const QVector3D &delta)
{
    const Qtr3dDblVector3D movement(delta);
    mCamera->lookAt(mCamera->pos() + movement, mCamera->lookAtCenter() + movement, QVector3D(0,1,0));
}

void Qtr3dFirstPersonCameraController::parentKeyPress(QKeyEvent *e)
{
    QVector3D delta;
    if (e->key() == Qt::Key_W || e->key() == Qt::Key_Up)
        delta += forward();
    if (e->key() == Qt::Key_S || e->key() == Qt::Key_Down)
        delta -= forward();
    if (e->key() == Qt::Key_A || e->key() == Qt::Key_Left)
        delta -= right();
    if (e->key() == Qt::Key_D || e->key() == Qt::Key_Right)
        delta += right();
    if (e->key() == Qt::Key_Space)
        delta += QVector3D(0,1,0);
    if (e->key() == Qt::Key_C)
        delta -= QVector3D(0,1,0);

    if (!delta.isNull())
        moveCamera(mMoveStep * delta.normalized());
}

void Qtr3dFirstPersonCameraController::parentMouseWheel(float delta)
{
    moveCamera((delta/120.0f) * mMoveStep * forward());
}

void Qtr3dFirstPersonCameraController::parentDraggingRel(QPointF delta)
{
    QVector3D dir = forward();
    QMatrix4x4 turnMatrix;
    turnMatrix.rotate(-delta.x() * 240, QVector3D(0,1,0));
    turnMatrix.rotate(-delta.y() * 240, right());
    dir = turnMatrix.mapVector(dir).normalized();
    mCamera->lookAt(mCamera->pos(), mCamera->pos() + Qtr3dDblVector3D(dir), QVector3D(0,1,0));
}
