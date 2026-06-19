#ifndef QTR3DABSTRACTSPACE_H
#define QTR3DABSTRACTSPACE_H

#include "qtr3dsimulationworld.h"
#include "qtr3dabstractentity.h"

class Qtr3dAbstractSpace : public Qtr3dSimulationWorld
{
public:
    explicit Qtr3dAbstractSpace(QObject *parent = nullptr);
    virtual ~Qtr3dAbstractSpace();
};

#endif // QTR3DABSTRACTSPACE_H
