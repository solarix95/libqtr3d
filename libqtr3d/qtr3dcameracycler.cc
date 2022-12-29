
#include <QMatrix4x4>
#include "qtr3dcameracycler.h"
#include "qtr3dcamera.h"

//-------------------------------------------------------------------------------------------------
Qtr3dCameraCycler::Qtr3dCameraCycler(Qtr3dCamera *cam, int fps, float deltaAngleY, const QVector3D &startPos, const QVector3D &lookAt)
 : Qtr3dCameraMovement(cam, fps)
 , mDeltaAngle(QVector3D(0,deltaAngleY,0))
 , mCurrentAngle(QVector3D(0,0,0))
 , mStartPos(startPos)
 , mLookAt(lookAt)
{
}

//-------------------------------------------------------------------------------------------------
Qtr3dCameraCycler::Qtr3dCameraCycler(Qtr3dCamera *cam, int fps, const QVector3D &deltaAngle, const QVector3D &startPos, const QVector3D &lookAt)
    : Qtr3dCameraMovement(cam, fps)
    , mDeltaAngle(deltaAngle)
    , mCurrentAngle(QVector3D(0,0,0))
    , mStartPos(startPos)
    , mLookAt(lookAt)
{

}

//-------------------------------------------------------------------------------------------------
void Qtr3dCameraCycler::process()
{
    mCurrentAngle.setX(mCurrentAngle.x() + mDeltaAngle.x());
    mCurrentAngle.setY(mCurrentAngle.y() + mDeltaAngle.y());
    mCurrentAngle.setZ(mCurrentAngle.z() + mDeltaAngle.z());

    mCurrentAngle += mDeltaAngle;
    QVector3D pointer = mStartPos-mLookAt;
    QMatrix4x4 matrix;
    matrix.rotate(mCurrentAngle.x(),QVector3D(1,0,0));
    matrix.rotate(mCurrentAngle.y(),QVector3D(0,1,0));
    matrix.rotate(mCurrentAngle.z(),QVector3D(0,0,1));

    pointer = pointer * matrix;

    camera()->lookAt(mLookAt + pointer,mLookAt,{0,1,0});
}

