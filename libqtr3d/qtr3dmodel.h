#ifndef QTR3DMODEL_H
#define QTR3DMODEL_H

#include <QObject>
#include "qtr3dgeometrybufferstate.h"

class Qtr3dModel : public Qtr3dGeometryBufferState
{
    Q_OBJECT
public:
    explicit Qtr3dModel();

    void addState(Qtr3dGeometryBufferState *state);

protected:
    Qtr3dGeometryBufferStates &states();

private:
    Qtr3dGeometryBufferStates mStates;
};

#endif // QTR3DMODEL_H
