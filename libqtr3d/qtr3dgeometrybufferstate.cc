#include "qtr3dgeometrybuffer.h"
#include "qtr3dgeometrybufferstate.h"

//------------------------------------------------------------------------------------------------
Qtr3dGeometryBufferState::Qtr3dGeometryBufferState(Qtr3dGeometryBuffer *parent, Qtr3d::LightingType ltype)
 : QObject(parent)
 , mBuffer(*parent)
 , mEnabled(true)
 , mLightingType(ltype)
{
    setState({0,0,0},{0,0,0},{1,1,1});
}

//------------------------------------------------------------------------------------------------
Qtr3dGeometryBufferState::Qtr3dGeometryBufferState(Qtr3dGeometryBuffer *parent, const QVector3D &pos, const QVector3D &rotation)
 : QObject(parent)
 , mBuffer(*parent)
 , mEnabled(true)
 , mLightingType(Qtr3d::DefaultLighting)
{
    setState(pos,rotation,{1,1,1});
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
void Qtr3dGeometryBufferState::setScale(float xyzScale)
{
    setScale({xyzScale,xyzScale,xyzScale});
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryBufferState::setRotation(const QVector3D &rotationAngles)
{
    mRot = rotationAngles;
    updateMatrix();
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryBufferState::setModelView(const QMatrix4x4 &modelView)
{
    if (mModelView == modelView)
        return;

    mModelView = modelView;
    mPos       = mModelView * QVector3D(0,0,0);
    emit updated();
}

//------------------------------------------------------------------------------------------------
float Qtr3dGeometryBufferState::radius() const
{
    return qMax(mScale.x(), qMax(mScale.y(), mScale.z())) * mBuffer.radius();
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
    emit updated();
}
