#ifndef QTR3DGEOMETRYBUFFER_H
#define QTR3DGEOMETRYBUFFER_H

#include <QObject>
#include <QList>
#include "qtr3dgeometrybufferstate.h"

class Qtr3dGeometryBufferState;

//-------------------------------------------------------------------------------------------------
class Qtr3dGeometryBuffer : public QObject
{
    Q_OBJECT
public:
    Qtr3dGeometryBuffer();

    // BufferState Iteration
    void registerBufferState(Qtr3dGeometryBufferState *s);
    inline const Qtr3dGeometryBufferStates &bufferStates() const { return mBufferStates; }

private slots:
    void stateDestroyed(QObject *state);

private:
    Qtr3dGeometryBufferStates mBufferStates;
};

typedef QList<Qtr3dGeometryBuffer> Qtr3dGeometryBuffers;

#endif // QTR3DGEOMETRYBUFFER_H
