#include <QtGlobal>
#include <QMatrix4x4>
#include "qtr3dmotionbody.h"
#include "qtr3dsimulationworld.h"
#include "qtr3dforcefield.h"

//-------------------------------------------------------------------------------------------------
Qtr3dMotionBody::Qtr3dMotionBody(Qtr3dGeometryState *state, QVector3D pos, QVector3D lookAt, QVector3D orientation, QObject *parent)
    : Qtr3dActor(state, parent)
    , mLookAt(lookAt)
    , mOrientation(orientation)
    , mVelocity(0,0,0)
    , mAngularVelocity(0,0,0)
    , mAcceleration(0,0,0)
    , mMass(1.0f)
    , mLinearDamping(0.0f)
    , mAngularDamping(0.0f)
{
    setPos(pos);
}

//-------------------------------------------------------------------------------------------------
Qtr3dMotionBody::~Qtr3dMotionBody() = default;

//-------------------------------------------------------------------------------------------------
QVector3D Qtr3dMotionBody::lookAt() const { return mLookAt; }
QVector3D Qtr3dMotionBody::orientation() const { return mOrientation; }
QVector3D Qtr3dMotionBody::velocity() const { return mVelocity; }
QVector3D Qtr3dMotionBody::movement() const { return velocity(); }
QVector3D Qtr3dMotionBody::angularVelocity() const { return mAngularVelocity; }
float Qtr3dMotionBody::mass() const { return mMass; }
float Qtr3dMotionBody::linearDamping() const { return mLinearDamping; }
float Qtr3dMotionBody::angularDamping() const { return mAngularDamping; }

//-------------------------------------------------------------------------------------------------
void Qtr3dMotionBody::setLookAt(const QVector3D &dir)
{
    if (mLookAt == dir)
        return;
    mLookAt = dir;
    emit lookAtChanged(dir);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMotionBody::setOrientation(const QVector3D &dir)
{
    if (mOrientation == dir)
        return;
    mOrientation = dir;
    emit orientationChanged(dir);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMotionBody::setVelocity(const QVector3D &velocity)
{
    if (mVelocity == velocity)
        return;
    mVelocity = velocity;
    emit velocityChanged(velocity);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMotionBody::setMovement(const QVector3D &velocity)
{
    setVelocity(velocity);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMotionBody::setAngularVelocity(const QVector3D &velocity)
{
    mAngularVelocity = velocity;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMotionBody::setAutorotation(const QVector3D &deltaAlpha)
{
    setAngularVelocity(deltaAlpha);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMotionBody::setMass(float mass)
{
    if (mass > 0.0f)
        mMass = mass;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMotionBody::setLinearDamping(float damping)
{
    mLinearDamping = qMax(0.0f, damping);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMotionBody::setAngularDamping(float damping)
{
    mAngularDamping = qMax(0.0f, damping);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMotionBody::applyAcceleration(const QVector3D &acceleration)
{
    mAcceleration += acceleration;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMotionBody::applyImpulse(const QVector3D &impulse)
{
    setVelocity(mVelocity + impulse / mMass);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMotionBody::relativeMove(float distance)
{
    if (!distance || mLookAt.isNull())
        return;
    setPos(pos() + (distance*mLookAt.normalized()));
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMotionBody::relativeStrafe(float distance)
{
    if (!distance || mLookAt.isNull() || mOrientation.isNull())
        return;
    setPos(pos() + distance*QVector3D::crossProduct(mLookAt,mOrientation).normalized());
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMotionBody::relativeTurn(float angle)
{
    if (!angle || mOrientation.isNull())
        return;
    QMatrix4x4 turnMatrix;
    turnMatrix.rotate(angle,mOrientation);
    setLookAt(turnMatrix.map(mLookAt));
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMotionBody::relativeRoll(float angle)
{
    if (!angle || mLookAt.isNull())
        return;
    QMatrix4x4 turnMatrix;
    turnMatrix.rotate(angle,mLookAt);
    setOrientation(turnMatrix.map(mOrientation));
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMotionBody::relativePitch(float angle)
{
    if (!angle || mLookAt.isNull() || mOrientation.isNull())
        return;
    QMatrix4x4 turnMatrix;
    turnMatrix.rotate(angle,QVector3D::crossProduct(mLookAt,mOrientation));
    setLookAt(turnMatrix.map(mLookAt));
    setOrientation(turnMatrix.map(mOrientation));
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dMotionBody::update(float dtSeconds)
{
    if (dtSeconds <= 0.0f)
        return false;

    QList<QVector3D> accelerations;
    centerAccelerations(accelerations);
    for (const auto &acceleration: accelerations)
        mAcceleration += acceleration;

    if (!mAcceleration.isNull())
        setVelocity(mVelocity + dtSeconds*mAcceleration);

    if (mLinearDamping > 0.0f)
        setVelocity(mVelocity / (1.0f + mLinearDamping*dtSeconds));

    if (!mAngularVelocity.isNull()) {
        setRotation(rotation() + dtSeconds*mAngularVelocity);
        if (mAngularDamping > 0.0f)
            mAngularVelocity /= (1.0f + mAngularDamping*dtSeconds);
    }

    mAcceleration = {0,0,0};
    return setPos(pos() + dtSeconds*mVelocity);
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dMotionBody::process(float simulationTimeMs, float normalizedSpeed)
{
    Q_UNUSED(normalizedSpeed);
    return update(simulationTimeMs/1000.0f);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMotionBody::centerAccelerations(QList<QVector3D> &accelerations) const
{
    centerForces(accelerations);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMotionBody::centerForces(QList<QVector3D> &accelerations) const
{
    if (world())
        world()->forceField().forceVector(pos(), accelerations);
}
