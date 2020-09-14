#include "qtr3dgeometrybuffer.h"

//-------------------------------------------------------------------------------------------------
Qtr3dGeometryBuffer::Qtr3dGeometryBuffer()
{
}

//-------------------------------------------------------------------------------------------------
void Qtr3dGeometryBuffer::registerBufferState(Qtr3dGeometryBufferState *s)
{
    Q_ASSERT(s);
    Q_ASSERT(!mBufferStates.contains(s));
    mBufferStates << s;
    connect(s, &Qtr3dGeometryBufferState::destroyed, this, &Qtr3dGeometryBuffer::stateDestroyed);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dGeometryBuffer::stateDestroyed(QObject *state)
{
    mBufferStates.removeOne((Qtr3dGeometryBufferState*)state);
}

