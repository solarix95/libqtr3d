
#include <libqtr3d/physics/qtr3dabstractspace.h>
#include "celestialbody.h"

CelestialBody::CelestialBody(Qtr3dGeometryState &state, float mass, QVector3D pos, QVector3D velocity)
    : Qtr3dStandardEntity(&state, pos, {0,0,0}, {0,0,0})
    , mMass(mass)
{
    setMovement(velocity);
}

float CelestialBody::mass() const
{
    return mMass;
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

        const CelestialBody *body = dynamic_cast<const CelestialBody*>(other);
        if (!body)
            continue;

        v.normalize();

        forces.append(v * (body->mass()/(d*d)));
    }
}
