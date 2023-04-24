#include "qtr3dstandardentity.h"
#include "qtr3dabstractspace.h"
#include "qtr3dforcefield.h"

//-------------------------------------------------------------------------------------------------
Qtr3dStandardEntity::Qtr3dStandardEntity(Qtr3dGeometryState &state, QVector3D pos, QVector3D lookAt, QVector3D orientation)
 : Qtr3dAbstractEntity(state)
 , mLookAt(lookAt)
 , mOrientation(orientation)
{
    setPos(pos);
}

//-------------------------------------------------------------------------------------------------
Qtr3dStandardEntity::~Qtr3dStandardEntity()
{
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dStandardEntity::process(float ms, float normalizedSpeed)
{
    QList<QVector3D> forces;
    centerForces(forces);

    for (auto f:forces)
        mMovement += ms*f;

    if (!mAutoRotation.isNull()) {
        state().setRotation(state().rot() + ms*mAutoRotation);
    }
    return setPos(pos() + mMovement);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dStandardEntity::centerForces(QList<QVector3D> &f) const
{
    space()->forceField().forceVector(pos(),f);
}

