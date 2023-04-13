#include "qtr3dcamera.h"

//-------------------------------------------------------------------------------------------------
Qtr3dCamera::Qtr3dCamera(Qtr3dWidget *widget)
 : QObject()
 , mWidget(widget)
 , mPos(QVector3D(0,0,0))
 , mFov(45.0)
 , mZNear(0.1)
 , mZFar(10000)
 , mWidth(16)
 , mHeight(9)
 , mLookAt(QVector3D(0,0,1))
 , mUp(0,1,0)
 , mAngles(QVector3D(0,0,0))
 , mMode(Fixed)
{
    Q_ASSERT(widget);
    updatePerspectiveMatrix();
}

//-------------------------------------------------------------------------------------------------
Qtr3dWidget *Qtr3dCamera::widget()
{
    return mWidget;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dCamera::setPos(float x, float y, float z)
{
    setPos(QVector3D(x,y,z));
}

//-------------------------------------------------------------------------------------------------
void Qtr3dCamera::setPos(const QVector3D &pos)
{
    if (mPos == pos)
        return;

    mPos = pos;
    updatePerspectiveMatrix();
    emit positionChanged(mPos);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dCamera::move(float dx, float dy, float dz)
{
    mPos += QVector3D(dx,dy,dz);
    updatePerspectiveMatrix();
    emit positionChanged(mPos);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dCamera::setFov(float angle)
{
    mFov = angle;
    updatePerspectiveMatrix();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dCamera::setFov(float angle, float zNear, float zFar)
{
    mFov   = angle;
    mZNear = zNear;
    mZFar  = zFar;
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
QVector3D Qtr3dCamera::pos() const
{
    return mPos;
}

//-------------------------------------------------------------------------------------------------
QVector3D Qtr3dCamera::lookAtCenter() const
{
    return mLookAt;
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
    emit positionChanged(mPos);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dCamera::lookAtTurn(float dxAngle, float dyAngle)
{
    if (mMode != LookAt)
        return;

    QVector3D vpos = mPos - mLookAt;

    QMatrix4x4 turnMatrix;

    turnMatrix.rotate(-dyAngle, QVector3D::crossProduct(vpos,mUp));
    turnMatrix.rotate(dxAngle, mUp);

    vpos = turnMatrix.map(vpos);
    mPos = mLookAt + vpos;
    updatePerspectiveMatrix();
    emit positionChanged(mPos);
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

    mProjectionMatrix.perspective(mFov,mWidth/(float)mHeight,mZNear,mZFar);

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
