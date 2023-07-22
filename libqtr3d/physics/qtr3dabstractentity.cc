#include "qtr3dabstractentity.h"

//-------------------------------------------------------------------------------------------------
Qtr3dAbstractEntity::Qtr3dAbstractEntity(Qtr3dGeometryState *state)
    : QObject(state)
    , mState(state)
{
}

//-------------------------------------------------------------------------------------------------
Qtr3dAbstractEntity::Qtr3dAbstractEntity(Qtr3dGeometryState *state, QObject *parent)
    : QObject(parent)
    , mState(state)
{
}

//-------------------------------------------------------------------------------------------------
Qtr3dAbstractEntity::~Qtr3dAbstractEntity() = default;

//-------------------------------------------------------------------------------------------------
bool Qtr3dAbstractEntity::setPos(const QVector3D &newPos)
{
    if (mPos == newPos)
        return false;
    mPos = newPos;
    onNewPosition();
    return true;
}

//-------------------------------------------------------------------------------------------------
float Qtr3dAbstractEntity::collisionRadius() const
{
    if (mState)
        mState->radius();
    return -1;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dAbstractEntity::onNewPosition()
{
    if (mState)
        mState->setPos(mPos);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dAbstractEntity::onNewState()
{
    if (mState)
        mState->setPos(mPos);
}


