#include "qtr3dcamera.h"

//-------------------------------------------------------------------------------------------------
Qtr3dCamera::Qtr3dCamera()
 : QObject()
 , mPos(QVector3D(0,0,0))
 , mFov(45.0)
 , mWidth(16)
 , mHeight(9)
 , mLookAt(QVector3D(0,0,1))
 , mUp(0,1,0)
 , mAngles(QVector3D(0,0,0))
 , mMode(Fixed)
{
    updatePerspectiveMatrix();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dCamera::setPosition(float x, float y, float z)
{
    setPosition(QVector3D(x,y,z));
    updatePerspectiveMatrix();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dCamera::setPosition(const QVector3D &pos)
{
    mPos = pos;
    updatePerspectiveMatrix();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dCamera::move(float dx, float dy, float dz)
{
    mPos += QVector3D(dx,dy,dz);
    updatePerspectiveMatrix();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dCamera::setFov(float angle)
{
    mFov = angle;
    updatePerspectiveMatrix();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dCamera::setGeometry(float width, float height)
{
    mWidth  = width;
    mHeight = height;
    Q_ASSERT(mHeight*mWidth > 0);
    updatePerspectiveMatrix();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dCamera::lookAt(const QVector3D &toCenter, const QVector3D &up)
{
    mLookAt = toCenter;
    mUp     = up;
    mMode   = LookAt;
    updatePerspectiveMatrix();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dCamera::lookAt(const QVector3D &pos, const QVector3D &toCenter, const QVector3D &up)
{
    mPos    = pos;
    mLookAt = toCenter;
    mUp     = up;
    mMode   = LookAt;
    updatePerspectiveMatrix();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dCamera::rotate(float xAngle, float yAngle, float zAngle)
{
    mAngles = QVector3D(xAngle, yAngle, zAngle);
    mMode   = Fixed;
    updatePerspectiveMatrix();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dCamera::updatePerspectiveMatrix()
{
    mWorldMatrix = mProjectionMatrix = QMatrix4x4();

    mProjectionMatrix.perspective(mFov,mWidth/(float)mHeight,1,-1);

    float height = 2;
    float width  = height * (mWidth/(float)mHeight);

    // mProjectionMatrix.frustum(-width/2,width/2,-height/2,height/2,10,-10);
    // qDebug() << mWidth << mHeight << width << height;

    switch(mMode) {
    case Fixed: {
        mWorldMatrix.translate(mPos);
        mWorldMatrix.rotate(mAngles.x(),1,0,0);
        mWorldMatrix.rotate(mAngles.y(),0,1,0);
        mWorldMatrix.rotate(mAngles.z(),0,0,1);
    } break;
    case LookAt: {
        mWorldMatrix.lookAt(mPos,mLookAt,mUp);
    } break;
    }

    // mWorldMatrix.optimize();
    emit changed();
}
