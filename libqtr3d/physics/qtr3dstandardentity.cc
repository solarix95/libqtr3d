#include "qtr3dstandardentity.h"

//-------------------------------------------------------------------------------------------------
Qtr3dStandardEntity::Qtr3dStandardEntity(Qtr3dGeometryState *state, QVector3D pos, QVector3D lookAt, QVector3D orientation)
    : Qtr3dMotionBody(state, pos, lookAt, orientation)
{
}

//-------------------------------------------------------------------------------------------------
Qtr3dStandardEntity::~Qtr3dStandardEntity() = default;
