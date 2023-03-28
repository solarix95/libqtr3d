#include "qtr3dentity.h"

Qtr3dEntity::Qtr3dEntity(Qtr3dGeometryBufferState &state)
 : QtrPhEntity({0,0,0}, {1,0,0}, {0,1,0}, &state)
 , mState(state)
{
    setCollisionRadius(state.radius());
}

void Qtr3dEntity::onNewPosition()
{
    mState.setPos(pos());
}
