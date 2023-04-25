#include <QMatrix4x4>
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
void Qtr3dStandardEntity::relativeMove(float distance)
{
    if (!distance)
        return;
    setPos(pos() + (distance*lookAt().normalized()));
}

//-------------------------------------------------------------------------------------------------
void Qtr3dStandardEntity::relativeStrafe(float distance)
{
    if (!distance)
        return;
    setPos(pos() + distance*QVector3D::crossProduct(lookAt(),orientation()).normalized());
}

//-------------------------------------------------------------------------------------------------
void Qtr3dStandardEntity::relativeTurn(float angle)
{
    if (!angle)
        return;
    QMatrix4x4 turnMatrix;
    turnMatrix.rotate(angle,orientation());
    setLookAt(turnMatrix.map(lookAt()));
}

//-------------------------------------------------------------------------------------------------
void Qtr3dStandardEntity::relativeRoll(float angle)
{
    if (!angle)
        return;
    QMatrix4x4 turnMatrix;
    turnMatrix.rotate(angle,lookAt());
    setOrientation(turnMatrix.map(orientation()));
}

//-------------------------------------------------------------------------------------------------
void Qtr3dStandardEntity::relativePitch(float angle)
{
    if (!angle)
        return;
    QMatrix4x4 turnMatrix;
    turnMatrix.rotate(angle,QVector3D::crossProduct(lookAt(),orientation()));

    setLookAt(turnMatrix.map(lookAt()));
    setOrientation(turnMatrix.map(orientation()));
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

