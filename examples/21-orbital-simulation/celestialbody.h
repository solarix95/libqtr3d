#ifndef CELESTIALBODY_H
#define CELESTIALBODY_H

#include "libqtr3d/physics/qtr3dstandardentity.h"

class CelestialBody : public Qtr3dStandardEntity
{
public:
    CelestialBody(Qtr3dGeometryState &state, float mass, QVector3D pos, QVector3D velocity);

    float mass() const;

protected:
    void centerForces(QList<QVector3D> &forces) const override;

private:
    float mMass;
};

#endif // CELESTIALBODY_H
