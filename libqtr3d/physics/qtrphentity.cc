#include <QDebug>
#include "qtrphentity.h"
#include "qtrphspace.h"
#include "qtr3dforcefield.h"

//-------------------------------------------------------------------------------------------------
QtrPhEntity::QtrPhEntity(QVector3D pos, QVector3D lookAt, QVector3D up, QObject *parent)
 : QObject(parent)
 , mPos(pos)
 , mLookAt(lookAt)
 , mUp(up)
{
}

//-------------------------------------------------------------------------------------------------
QtrPhEntity::~QtrPhEntity()
{
}

//-------------------------------------------------------------------------------------------------
bool QtrPhEntity::process(float ms, float normalizedSpeed)
{
    QList<QVector3D> forces;
    centerForces(forces);

    for (auto f:forces)
        mMovement += ms*f;

    return setPos(mPos + mMovement);
}

//-------------------------------------------------------------------------------------------------
void QtrPhEntity::setCollisionRadius(float radius)
{
    mCollisionRadius = radius;
}

//-------------------------------------------------------------------------------------------------
void QtrPhEntity::centerForces(QList<QVector3D> &forces) const
{
    mSpace->forceField()->forceVector(pos(),forces);
}

//-------------------------------------------------------------------------------------------------
void QtrPhEntity::onNewPosition()
{
}

//-------------------------------------------------------------------------------------------------
bool QtrPhEntity::setPos(const QVector3D &newPos)
{
    if (mPos == newPos)
        return false;
    mPos = newPos;
    onNewPosition();
    return true;
}
