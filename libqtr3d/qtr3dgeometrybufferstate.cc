#include "qtr3dgeometrybufferstate.h"

//------------------------------------------------------------------------------------------------
Qtr3dGeometryBufferState::Qtr3dGeometryBufferState(bool isFlat)
 : mIsFlat(isFlat)
 , mParent(nullptr)
{
    setState({0,0,0},{0,0,0},{1,1,1});
}

//------------------------------------------------------------------------------------------------
Qtr3dGeometryBufferState::Qtr3dGeometryBufferState(const QVector3D &pos, const QVector3D &rotation)
 : mIsFlat(true)
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
void Qtr3dGeometryBufferState::setFlat(bool flat)
{
    mIsFlat = flat;
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
