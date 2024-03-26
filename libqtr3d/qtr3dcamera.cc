#include "qtr3dcamera.h"

//-------------------------------------------------------------------------------------------------
Qtr3dCamera::Qtr3dCamera(Qtr3dWidget *widget)
 : QObject()
 , mWidget(widget)

 , mFov(45.0)
 , mZNear(0.1)
 , mZFar(10000)
 , mWidth(16)
 , mHeight(9)

 , mPos(Qtr3dDblVector3D{0.0,0.0,0.0})
 , mLookAt(QVector3D(0,0,1))
 , mUp(0,1,0)

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
    setPos(Qtr3dDblVector3D(x,y,z));
}

//-------------------------------------------------------------------------------------------------
void Qtr3dCamera::setPos(double x, double y, double z)
{
    setPos(Qtr3dDblVector3D(x,y,z));
}

//-------------------------------------------------------------------------------------------------
void Qtr3dCamera::setPos(const Qtr3dDblVector3D &pos)
{
    if (mPos == pos)
        return;

    mPos = pos;
    emit changed();
    emit positionChanged(mPos);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dCamera::move(float dx, float dy, float dz)
{
    mPos += QVector3D(dx,dy,dz);
    emit changed();
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
Qtr3dDblVector3D Qtr3dCamera::pos() const
{
    return mPos;
}

//-------------------------------------------------------------------------------------------------
Qtr3dDblVector3D Qtr3dCamera::lookAtCenter() const
{
    return mLookAt;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dCamera::lookAt(const Qtr3dDblVector3D &toCenter, const QVector3D &up)
{
    mLookAt = toCenter;
    mUp     = up;
    emit changed();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dCamera::lookAt(const Qtr3dDblVector3D &pos, const Qtr3dDblVector3D &toCenter, const QVector3D &up)
{
    bool changedPos = mPos != pos;
    mPos    = pos;
    mLookAt = toCenter;
    mUp     = up;

    emit changed();
    if (changedPos)
        emit positionChanged(mPos);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dCamera::setupWorldMatrix(QMatrix4x4 &matrix, bool absolutPos)
{
    matrix.setToIdentity();
    if (absolutPos)
        matrix.lookAt(mPos.toFloat(),mLookAt.toFloat(),mUp);
    else
        matrix.lookAt({0,0,0},(mLookAt-mPos).toFloat(),mUp);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dCamera::lookAtTurn(float dxAngle, float dyAngle)
{
    auto vpos = mPos - mLookAt;

    QMatrix4x4 turnMatrix;

    turnMatrix.rotate(dyAngle, QVector3D::crossProduct(vpos.toFloat(),mUp));
    turnMatrix.rotate(-dxAngle, mUp);

    vpos = turnMatrix.map(vpos);
    mPos = mLookAt + vpos;

    emit changed();
    emit positionChanged(mPos);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dCamera::rotate(float xAngle, float yAngle, float zAngle)
{
    Q_ASSERT(0);
}

//-------------------------------------------------------------------------------------------------
const QMatrix4x4 Qtr3dCamera::worldView(bool withPosition) const
{
    QMatrix4x4 m;
    if (withPosition)
        m.lookAt(mPos ,mLookAt,mUp);
    else
        m.lookAt(Qtr3dDblVector3D{0,0,0},mLookAt-mPos,mUp);
    return m;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dCamera::updatePerspectiveMatrix()
{
    mProjectionMatrix.perspective(mFov,mWidth/(float)mHeight,mZNear,mZFar);
    emit changed();
}
