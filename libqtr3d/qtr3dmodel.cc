#include "qtr3dmodel.h"

//-------------------------------------------------------------------------------------------------
Qtr3dModel::Qtr3dModel()
    : Qtr3dGeometryBufferState()
{
}

//-------------------------------------------------------------------------------------------------
void Qtr3dModel::addState(Qtr3dGeometryBufferState *state)
{
    Q_ASSERT(state);
    mStates << state;
    state->setParent(this);
}

//-------------------------------------------------------------------------------------------------
Qtr3dGeometryBufferStates &Qtr3dModel::states()
{
    return mStates;
}
