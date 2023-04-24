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
    // float targetDistance      = mDistance > 0 ? mDistance : ((mCamera->pos() - mCamera->lookAtCenter()).length());
    QVector3D targetPos       = (mState->pos() -  mDistance*targetDirection) + mOffset*mTargetOrientation;


    QVector3D cameraVector    = targetPos - mCamera->pos();
    QVector3D newCameraPos    = mCamera->pos()  + 0.01*(cameraVector);

    bool posIsStable = ((mCamera->pos() - newCameraPos).length()    < (mCamera->pos() - mCamera->lookAtCenter()).length()*0.005);
    //bool cenIsStable = ((targetPos      - newCameraCenter).length() < (mCamera->pos() - mCamera->lookAtCenter()).length()*0.005);

    //if (posIsStable && cenIsStable)
    //    return;

    // qDebug() << mCamera->lookAtCenter() << targetDirection;
    auto lastCameraDirection = (mCamera->lookAtCenter() - mCamera->pos()).normalized();

    // "Over the shoulder"
    // auto newCameraDirection  = lastCameraDirection + (0.005*(targetDirection-lastCameraDirection));
    auto targetCameraDirection  = (mState->pos() - targetPos).normalized();
    auto newCameraDirection  = lastCameraDirection + (0.01*(targetCameraDirection-lastCameraDirection));

    // QVector3D newCameraCenter  = (lastCameraCenter + 0.01*(targetDirection-lastCameraCenter));
    mCamera->lookAt(newCameraPos, newCameraPos + newCameraDirection, mTargetOrientation);
}
