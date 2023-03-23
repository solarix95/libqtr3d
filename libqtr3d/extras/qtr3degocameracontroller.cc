
#include <QEvent>
#include <QDebug>
#include "qtr3degocameracontroller.h"
#include "../qtr3dwidget.h"
#include "../qtr3dcamera.h"

//-------------------------------------------------------------------------------------------------
Qtr3dEgoCameraController::Qtr3dEgoCameraController(Qtr3dWidget *parent)
 : Qtr3dWidgetInputController(parent)
{
    Q_ASSERT(parent);
    parent->installEventFilter(this);

    mCamera = parent->camera();
    Q_ASSERT(mCamera);
}

Qtr3dEgoCameraController::~Qtr3dEgoCameraController() = default;

//-------------------------------------------------------------------------------------------------
void Qtr3dEgoCameraController::parentKeyPress(QKeyEvent *e)
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
void Qtr3dEgoCameraController::parentMouseWheel(float delta)
{
    float correctionFactor = (360+(-1*delta))/360.0;

    QVector3D vpos = correctionFactor * (mCamera->pos() - mCamera->lookAtCenter());
    mCamera->setPosition(mCamera->lookAtCenter() + vpos);
    emit positionChanged(mCamera->pos());
}

//-------------------------------------------------------------------------------------------------
void Qtr3dEgoCameraController::parentDraggingRel(QPointF delta)
{
    qDebug() << delta;
    static double angleY = 0;
    static double angleZ = 0;
    angleY += 50*delta.x();
    angleZ += 50*delta.y();

    mCamera->rotate(angleZ,angleY,0);
    // mCamera->lookAtTurn(delta.x() * 300,delta.y() * 300);
}
