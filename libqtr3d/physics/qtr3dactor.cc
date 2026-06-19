#include "qtr3dactor.h"

//-------------------------------------------------------------------------------------------------
Qtr3dActor::Qtr3dActor(Qtr3dGeometryState *state, QObject *parent)
    : QObject(parent ? parent : state)
    , mPos(0,0,0)
    , mRotation(0,0,0)
    , mScale(1,1,1)
    , mWorld(nullptr)
    , mState(state)
{
    onNewState();
}

//-------------------------------------------------------------------------------------------------
Qtr3dActor::~Qtr3dActor() = default;

//-------------------------------------------------------------------------------------------------
void Qtr3dActor::setState(Qtr3dGeometryState *state)
{
    if (mState == state)
        return;
    mState = state;
    onNewState();
    emit stateChanged(state);
}

//-------------------------------------------------------------------------------------------------
Qtr3dGeometryState *Qtr3dActor::state() const
{
    return mState;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dActor::setWorld(Qtr3dSimulationWorld *world)
{
    mWorld = world;
}

//-------------------------------------------------------------------------------------------------
Qtr3dSimulationWorld *Qtr3dActor::world() const
{
    return mWorld;
}

//-------------------------------------------------------------------------------------------------
QVector3D Qtr3dActor::pos() const
{
    return mPos;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dActor::setPos(const QVector3D &newPos)
{
    if (mPos == newPos)
        return false;
    mPos = newPos;
    onNewPosition();
    emit positionChanged(mPos);
    return true;
}

//-------------------------------------------------------------------------------------------------
QVector3D Qtr3dActor::rotation() const
{
    return mRotation;
}

//-------------------------------------------------------------------------------------------------
QVector3D Qtr3dActor::rot() const
{
    return rotation();
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dActor::setRotation(const QVector3D &rotation)
{
    if (mRotation == rotation)
        return false;
    mRotation = rotation;
    onNewRotation();
    emit rotationChanged(mRotation);
    return true;
}

//-------------------------------------------------------------------------------------------------
QVector3D Qtr3dActor::scale() const
{
    return mScale;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dActor::setScale(const QVector3D &scale)
{
    if (mScale == scale)
        return false;
    mScale = scale;
    onNewScale();
    emit scaleChanged(mScale);
    return true;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dActor::setScale(float xyzScale)
{
    return setScale({xyzScale, xyzScale, xyzScale});
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dActor::update(float dtSeconds)
{
    Q_UNUSED(dtSeconds);
    return false;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dActor::process(float simulationTimeMs, float normalizedSpeed)
{
    Q_UNUSED(normalizedSpeed);
    return update(simulationTimeMs/1000.0f);
}

//-------------------------------------------------------------------------------------------------
float Qtr3dActor::collisionRadius() const
{
    if (mState)
        return mState->radius();
    return -1.0f;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dActor::onNewPosition()
{
    if (mState)
        mState->setPos(mPos);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dActor::onNewRotation()
{
    if (mState)
        mState->setRotation(mRotation);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dActor::onNewScale()
{
    if (mState)
        mState->setScale(mScale);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dActor::onNewState()
{
    onNewPosition();
    onNewRotation();
    onNewScale();
}
