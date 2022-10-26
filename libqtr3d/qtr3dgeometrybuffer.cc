#include "qtr3dgeometrybuffer.h"

//-------------------------------------------------------------------------------------------------
Qtr3dGeometryBuffer::Qtr3dGeometryBuffer()
{
}

QVector3D Qtr3dGeometryBuffer::minValues() const
{
    return QVector3D(0,0,0);
}

QVector3D Qtr3dGeometryBuffer::maxValues() const
{
    return QVector3D(0,0,0);
}

//-------------------------------------------------------------------------------------------------
QVector3D Qtr3dGeometryBuffer::center() const
{
    return QVector3D(
                (maxValues().x() + minValues().x())/2,
                (maxValues().y() + minValues().y())/2,
                (maxValues().z() + minValues().z())/2
                );
}

//-------------------------------------------------------------------------------------------------
double Qtr3dGeometryBuffer::radius() const
{
    return qMax(maxValues().x() - minValues().x(),qMax(maxValues().y() - minValues().y(),maxValues().z() - minValues().z()));
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

