#ifndef QTR3DENTITY_H
#define QTR3DENTITY_H

#include <QObject>
#include "qtrphentity.h"
#include "../qtr3dgeometrybufferstate.h"

class Qtr3dEntity : public QtrPhEntity
{
public:
    Qtr3dEntity(Qtr3dGeometryBufferState &state);

protected:
    virtual void onNewPosition();

private:
    Qtr3dGeometryBufferState &mState;
};

#endif // QTR3DENTITY_H
