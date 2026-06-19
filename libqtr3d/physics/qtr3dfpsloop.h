#ifndef QTR3DFPSLOOP_H
#define QTR3DFPSLOOP_H

#include "qtr3dsimulationloop.h"

class Qtr3dFpsLoop : public Qtr3dSimulationLoop
{
public:
    explicit Qtr3dFpsLoop(QObject *parent = nullptr);
    virtual ~Qtr3dFpsLoop();
};

#endif // QTR3DFPSLOOP_H
