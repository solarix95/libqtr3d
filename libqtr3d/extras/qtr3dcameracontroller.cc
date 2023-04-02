
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
    Qtr3dWidgetInputController::parentKeyPress(e);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dFreeCameraController::parentMouseWheel(float delta)
{
    float correctionFactor = delta/120;

    if (modifiers().testFlag(Qt::ControlModifier))
        correctionFactor *= 0.1;

    QVector3D vpos = 0.1*(mCamera->lookAtCenter() - mCamera->pos());
    mCamera->setPos(mCamera->pos() + (correctionFactor*vpos));
    emit positionChanged(mCamera->pos());
}

//-------------------------------------------------------------------------------------------------
void Qtr3dFreeCameraController::parentDraggingRel(QPointF delta)
{
    mCamera->lookAtTurn(delta.x() * 300,delta.y() * 300);
}
