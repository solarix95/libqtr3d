#include "qtr3dgeometrybufferstate.h"

//------------------------------------------------------------------------------------------------
Qtr3dGeometryBufferState::Qtr3dGeometryBufferState(Qtr3d::LightingType ltype)
 : mEnabled(true)
 , mLightingType(ltype)
 , mParent(nullptr)
{
    setState({0,0,0},{0,0,0},{1,1,1});
}

//------------------------------------------------------------------------------------------------
Qtr3dGeometryBufferState::Qtr3dGeometryBufferState(const QVector3D &pos, const QVector3D &rotation)
 : mEnabled(true)
 , mLightingType(Qtr3d::DefaultLighting)
 , mParent(nullptr)
{
    setState(pos,rotation,{1,1,1});
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryBufferState::setParent(Qtr3dGeometryBufferState *state)
{
    mParent = state;
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryBufferState::setLightingType(Qtr3d::LightingType ltype)
{
    mLightingType = ltype;
}

//------------------------------------------------------------------------------------------------
Qtr3d::LightingType Qtr3dGeometryBufferState::lightingType() const
{
    return mLightingType;
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryBufferState::setEnabled(bool enabled)
{
    mEnabled = enabled;
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryBufferState::setPos(const QVector3D &pos)
{
    mPos = pos;
    updateMatrix();
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryBufferState::move(const QVector3D &pos)
{
    mPos = pos;
    updateMatrix();
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryBufferState::move(const QVector3D &pos, const QVector3D &rotation)
{
    mPos = pos;
    mRot = rotation;
    updateMatrix();
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryBufferState::setState(const QVector3D &pos, const QVector3D &rotation, const QVector3D &scale)
{
    mPos = pos;
    mRot = rotation;
    mScale = scale;
    updateMatrix();
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryBufferState::setScale(const QVector3D &scale)
{
    mScale = scale;
    updateMatrix();
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryBufferState::setRotation(const QVector3D &rotationAngles)
{
    mRot = rotationAngles;
    updateMatrix();
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryBufferState::updateMatrix()
{
    mModelView = QMatrix4x4();
    mModelView.data()[15] = 1.0;

    mModelView.translate(mPos);

    mModelView.rotate(mRot.x(),1,0,0);
    mModelView.rotate(mRot.y(),0,1,0);
    mModelView.rotate(mRot.z(),0,0,1);

    mModelView.scale(mScale);
}
