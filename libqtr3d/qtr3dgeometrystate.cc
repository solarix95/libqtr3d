#include "qtr3dgeometry.h"
#include "qtr3dgeometrystate.h"
#include "qtr3dmodelanimator.h"

//------------------------------------------------------------------------------------------------
Qtr3dGeometryState::Qtr3dGeometryState(Qtr3dGeometry *parent, Qtr3d::LightingType ltype)
 : QObject(parent)
 , mBuffer(*parent)
 , mEnabled(true)
 , mLightingType(ltype)
 , mAnimator(nullptr)
{
    setState({0,0,0},{0,0,0},{1,1,1});
}

//------------------------------------------------------------------------------------------------
Qtr3dGeometryState::Qtr3dGeometryState(Qtr3dGeometry *parent, const QVector3D &pos, const QVector3D &rotation)
 : QObject(parent)
 , mBuffer(*parent)
 , mEnabled(true)
 , mLightingType(Qtr3d::DefaultLighting)
 , mAnimator(nullptr)
{
    setState(pos,rotation,{1,1,1});
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::setLightingType(Qtr3d::LightingType ltype)
{
    mLightingType = ltype;
}

//------------------------------------------------------------------------------------------------
Qtr3d::LightingType Qtr3dGeometryState::lightingType() const
{
    return mLightingType;
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::setEnabled(bool enabled)
{
    mEnabled = enabled;
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::setPos(const QVector3D &pos)
{
    mPos = pos;
    updateMatrix();
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::move(const QVector3D &pos)
{
    mPos = pos;
    updateMatrix();
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::move(const QVector3D &pos, const QVector3D &rotation)
{
    mPos = pos;
    mRot = rotation;
    updateMatrix();
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::setState(const QVector3D &pos, const QVector3D &rotation, const QVector3D &scale)
{
    mPos = pos;
    mRot = rotation;
    mScale = scale;
    updateMatrix();
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::setScale(const QVector3D &scale)
{
    mScale = scale;
    updateMatrix();
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::setScale(float xyzScale)
{
    setScale({xyzScale,xyzScale,xyzScale});
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::setRotation(const QVector3D &rotationAngles)
{
    mRot = rotationAngles;
    updateMatrix();
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::setModelView(const QMatrix4x4 &modelView)
{
    if (mModelView == modelView)
        return;

    mModelView = modelView;
    mPos       = mModelView * QVector3D(0,0,0);
    emit updated();
}

//------------------------------------------------------------------------------------------------
float Qtr3dGeometryState::radius() const
{
    return qMax(mScale.x(), qMax(mScale.y(), mScale.z())) * mBuffer.radius();
}

//------------------------------------------------------------------------------------------------
QVector3D Qtr3dGeometryState::center() const
{
    return mPos + (mScale * mBuffer.center());
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::setAnimator(Qtr3dModelAnimator *animator)
{
    if (mAnimator)
        mAnimator->deleteLater();
    mAnimator = animator;
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::updateMatrix()
{
    mModelView = QMatrix4x4();
    mModelView.setToIdentity();
    // mModelView.data()[15] = 1.0;

    mModelView.translate(mPos);

    mModelView.rotate(mRot.x(),1,0,0);
    mModelView.rotate(mRot.y(),0,1,0);
    mModelView.rotate(mRot.z(),0,0,1);

    mModelView.scale(mScale);
    emit updated();
}
