
#include <libqtr3d/physics/qtr3dabstractspace.h>
#include "celestialbody.h"

CelestialBody::CelestialBody(Qtr3dGeometryState &state, float weight, QVector3D pos, QVector3D direction)
    : Qtr3dStandardEntity(&state, pos, {0,0,0}, {0,0,0})
    , mWeight(weight)
{
    setMovement(direction);
}

void CelestialBody::centerForces(QList<QVector3D> &forces) const
{
    const Qtr3dAbstractEntities &others = space()->entities();
    for (auto *other : others) {
        if (other == this)
            continue;

        QVector3D v = (other->pos() - pos());
        float     d = v.length();
        if (d <= 0)
            continue;

        v.normalize();

        forces.append( v * ((((CelestialBody*)other)->mWeight)/(d*d)) );
    }
}
