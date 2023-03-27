#include "qtrphentity.h"

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
bool QtrPhEntity::process(float /*ms*/, float normalizedSpeed)
{
    QList<QVector3D> forces;
    centerForces(forces);

    for (auto f:forces)
        mMovement += normalizedSpeed*f;

    return setPos(mPos + mMovement);
}

//-------------------------------------------------------------------------------------------------
void QtrPhEntity::centerForces(QList<QVector3D> &/*forces*/) const
{
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
