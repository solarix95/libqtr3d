#include "qtr3dabstractentity.h"

//-------------------------------------------------------------------------------------------------
Qtr3dAbstractEntity::Qtr3dAbstractEntity(Qtr3dGeometryState &state)
 : QObject(&state)
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
    return mState.radius();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dAbstractEntity::onNewPosition()
{
    mState.setPos(mPos);
}


