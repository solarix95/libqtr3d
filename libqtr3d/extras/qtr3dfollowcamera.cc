#include <QTimer>
#include "../qtr3dcamera.h"
#include "../qtr3dwidget.h"
#include "../qtr3dassets.h"
#include "../qtr3dgeometrystate.h"
#include "../physics/qtr3dfpsloop.h"
#include "qtr3dfollowcamera.h"

//-------------------------------------------------------------------------------------------------
Qtr3dFollowCamera::Qtr3dFollowCamera(Qtr3dWidget *widget, Qtr3dGeometryState *followState, float distance, float offset)
 : QObject(widget)
 , mState(followState)
 , mDistance(distance)
 , mTargetOrientation({0,1,0})
 , mOffset(offset)
 , mPosController(QVector3D(0.1,0.1,0.1),QVector3D(0.00001,0.00001,0.0001),QVector3D(1,1,1))
 , mDirController(QVector3D(0.1,0.1,0.1),QVector3D(0.00001,0.00001,0.00001),QVector3D(1,1,1))
{
    mCamera = widget->camera();
    connect(&widget->assets()->loop(), &Qtr3dFpsLoop::stepDone, this, &Qtr3dFollowCamera::process);
}

//-------------------------------------------------------------------------------------------------
Qtr3dFollowCamera::~Qtr3dFollowCamera()
{
}

//-------------------------------------------------------------------------------------------------
void Qtr3dFollowCamera::setTargetDirection(const QVector3D &direction)
{
    mTargetDirection = direction;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dFollowCamera::setTargetOrientation(const QVector3D &orientation)
{
    mTargetOrientation = orientation.normalized();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dFollowCamera::setOffset(float offset)
{
    mOffset = offset;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dFollowCamera::process()
{
    follow();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dFollowCamera::follow()
{
    QVector3D targetDirection = (mTargetDirection.isNull() ? (mState->pos()-mCamera->pos()) : mTargetDirection).normalized();
    QVector3D targetPos       = (mState->pos() -  mDistance*targetDirection) + mOffset*mTargetOrientation;

    // bool posIsStable = ((mCamera->pos() - newCameraPos).length()    < (mCamera->pos() - mCamera->lookAtCenter()).length()*0.005);
    //bool cenIsStable = ((targetPos      - newCameraCenter).length() < (mCamera->pos() - mCamera->lookAtCenter()).length()*0.005);

    //if (posIsStable && cenIsStable)
    //    return;

    auto lastCameraDirection    = (mCamera->lookAtCenter() - mCamera->pos()).normalized();
    auto targetCameraDirection  = (mState->pos() - targetPos).normalized();

    auto newCameraPos       = mCamera->pos()      + mPosController.calculate(targetPos,mCamera->pos(),20);
    auto newCameraDirection = lastCameraDirection + mDirController.calculate(targetCameraDirection,lastCameraDirection,20);
    mCamera->lookAt(newCameraPos, newCameraPos + newCameraDirection, mTargetOrientation);

}
