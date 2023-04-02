#ifndef CELESTIALBODY_H
#define CELESTIALBODY_H

#include "libqtr3d/physics/qtr3dstandardentity.h"

class CelestialBody : public Qtr3dStandardEntity
{
public:
    CelestialBody(Qtr3dGeometryState &state, float weight, QVector3D pos, QVector3D direction);

protected:
     virtual void centerForces(QList<QVector3D> &forces) const;

private:
    float mWeight;
};

#endif // CELESTIALBODY_H
