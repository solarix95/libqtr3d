
#include <QMatrix4x4>
#include "qtr3dcameracycler.h"
#include "qtr3dcamera.h"
//-------------------------------------------------------------------------------------------------
Qtr3dCameraCycler::Qtr3dCameraCycler(Qtr3dCamera *cam, int fps, float deltaAngle, const QVector3D &startPos, const QVector3D &lookAt)
 : Qtr3dCameraMovement(cam, fps)
 , mDeltaAngle(deltaAngle)
 , mCurrentAngle(0)
 , mStartPos(startPos)
 , mLookAt(lookAt)
{
}

//-------------------------------------------------------------------------------------------------
void Qtr3dCameraCycler::process()
{
    mCurrentAngle += mDeltaAngle;
    QVector3D pointer = mStartPos-mLookAt;
    QMatrix4x4 matrix;
    matrix.rotate(mCurrentAngle,QVector3D(0,1,0));
    pointer = pointer * matrix;

    camera()->lookAt(mLookAt + pointer,mLookAt,{0,1,0});
}

