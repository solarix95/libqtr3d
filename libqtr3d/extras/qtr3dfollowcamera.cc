#include <QTimer>
#include "../qtr3dcamera.h"
#include "../qtr3dwidget.h"
#include "../qtr3dassets.h"
#include "../qtr3dgeometrystate.h"
#include "../physics/qtr3dfpsloop.h"
#include "qtr3dfollowcamera.h"

//-------------------------------------------------------------------------------------------------
Qtr3dFollowCamera::Qtr3dFollowCamera(Qtr3dWidget *widget, Qtr3dGeometryState *followState, float distance)
 : QObject(widget)
 , mState(followState)
 , mDistance(distance)
{
    mCamera = widget->camera();
    connect(&widget->assets()->loop(), &Qtr3dFpsLoop::stepDone, this, &Qtr3dFollowCamera::process);
}

//-------------------------------------------------------------------------------------------------
Qtr3dFollowCamera::~Qtr3dFollowCamera()
{
}

//-------------------------------------------------------------------------------------------------
void Qtr3dFollowCamera::process()
{
    follow();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dFollowCamera::follow()
{
    float targetDistance = mDistance > 0 ? mDistance : ((mCamera->pos() - mCamera->lookAtCenter()).length());

    QVector3D newCameraCenter = mCamera->lookAtCenter() + 0.01*(mState->pos()-mCamera->lookAtCenter());
    QVector3D cameraVector    = mState->pos() - mCamera->pos();
    cameraVector              = (1-(targetDistance/cameraVector.length())) * cameraVector;

    QVector3D newCameraPos    = mCamera->pos()  + 0.01*(cameraVector);

   // float dist = (mCamera->pos() - mCamera->lookAtCenter()).length();

    bool posIsStable = ((mCamera->pos() - newCameraPos).length()    < (mCamera->pos() - mCamera->lookAtCenter()).length()*0.005);
    bool cenIsStable = ((mState->pos()  - newCameraCenter).length() < (mCamera->pos() - mCamera->lookAtCenter()).length()*0.005);

    if (posIsStable && cenIsStable)
        return;

    mCamera->lookAt(newCameraPos,newCameraCenter, {0,1,0});
}
