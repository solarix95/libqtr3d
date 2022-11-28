
#include <QEvent>
#include <QDebug>
#include "qtr3dfreecameracontroller.h"
#include "../qtr3dwidget.h"
#include "../qtr3dcamera.h"

//-------------------------------------------------------------------------------------------------
Qtr3dFreeCameraController::Qtr3dFreeCameraController(Qtr3dWidget *parent)
 : Qtr3dWidgetInputController(parent)
{
    Q_ASSERT(parent);
    parent->installEventFilter(this);

    mCamera = parent->camera();
    Q_ASSERT(mCamera);
}

Qtr3dFreeCameraController::~Qtr3dFreeCameraController() = default;

//-------------------------------------------------------------------------------------------------
void Qtr3dFreeCameraController::parentKeyPress(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Up)
        mCamera->move(0,0.1,0);
    if (e->key() == Qt::Key_Down)
        mCamera->move(0,-0.1,0);
    if (e->key() == Qt::Key_Left)
        mCamera->move(-0.1,0,0);
    if (e->key() == Qt::Key_Right)
        mCamera->move(0.1,0,0);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dFreeCameraController::parentMouseWheel(float delta)
{
    float correctionFactor = (360+(-1*delta))/360.0;

    QVector3D vpos = correctionFactor * (mCamera->pos() - mCamera->lookAtCenter());
    mCamera->setPosition(mCamera->lookAtCenter() + vpos);
    emit positionChanged(mCamera->pos());
}

//-------------------------------------------------------------------------------------------------
void Qtr3dFreeCameraController::parentDraggingRel(QPointF delta)
{
    mCamera->lookAtTurn(delta.x() * 300,delta.y() * 300);
}
