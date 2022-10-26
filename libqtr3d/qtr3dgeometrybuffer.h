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
    enum Type {
        Unknown,
        Dot,
        Line,
        Triangle
    };

    Qtr3dGeometryBuffer();

    virtual QVector3D minValues() const; // lowest xyz
    virtual QVector3D maxValues() const; // max xyz
    virtual QVector3D center() const;
    virtual double    radius() const;

    // BufferState Iteration
    virtual void registerBufferState(Qtr3dGeometryBufferState *s);
    inline const Qtr3dGeometryBufferStates &bufferStates() const { return mBufferStates; }

private slots:
    void stateDestroyed(QObject *state);

private:
    Qtr3dGeometryBufferStates mBufferStates;
};

typedef QList<Qtr3dGeometryBuffer> Qtr3dGeometryBuffers;

#endif // QTR3DGEOMETRYBUFFER_H
