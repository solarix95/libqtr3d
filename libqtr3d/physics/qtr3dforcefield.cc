#include "qtr3dforcefield.h"

//-------------------------------------------------------------------------------------------------
Qtr3dForceField::Qtr3dForceField(QObject *parent)
 : QObject(parent)
{
}

//-------------------------------------------------------------------------------------------------
void Qtr3dForceField::setConstantForces(const QList<QVector3D> &forces)
{
    mConstantForces =  forces;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dForceField::setConstantForce(const QVector3D &force)
{
    mConstantForces = QList<QVector3D>() << force;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dForceField::forceVector(const QVector3D &pos, QList<QVector3D> &forces)
{
    forces = mConstantForces;
}
