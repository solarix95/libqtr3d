#ifndef QTR3DMOTIONBODY_H
#define QTR3DMOTIONBODY_H

#include <QList>
#include <QVector3D>
#include "qtr3dactor.h"

class Qtr3dMotionBody : public Qtr3dActor
{
    Q_OBJECT
public:
    Qtr3dMotionBody(Qtr3dGeometryState *state, QVector3D pos = {0,0,0}, QVector3D lookAt = {0,0,-1}, QVector3D orientation = {0,1,0}, QObject *parent = nullptr);
    virtual ~Qtr3dMotionBody();

    QVector3D lookAt() const;
    QVector3D orientation() const;
    QVector3D velocity() const;
    QVector3D movement() const;
    QVector3D angularVelocity() const;
    float mass() const;
    float linearDamping() const;
    float angularDamping() const;

    void setLookAt(const QVector3D &dir);
    void setOrientation(const QVector3D &dir);
    void setVelocity(const QVector3D &velocity);
    void setMovement(const QVector3D &velocity);
    void setAngularVelocity(const QVector3D &velocity);
    void setAutorotation(const QVector3D &deltaAlpha);
    void setMass(float mass);
    void setLinearDamping(float damping);
    void setAngularDamping(float damping);

    void applyAcceleration(const QVector3D &acceleration);
    void applyImpulse(const QVector3D &impulse);

    void relativeMove(float distance);
    void relativeStrafe(float distance);
    void relativeTurn(float angle);
    void relativeRoll(float angle);
    void relativePitch(float angle);

    bool update(float dtSeconds) override;
    bool process(float simulationTimeMs, float normalizedSpeed) override;

signals:
    void lookAtChanged(const QVector3D &newLookAt);
    void orientationChanged(const QVector3D &newOrientation);
    void velocityChanged(const QVector3D &velocity);

protected:
    virtual void centerAccelerations(QList<QVector3D> &accelerations) const;
    virtual void centerForces(QList<QVector3D> &accelerations) const;

private:
    QVector3D mLookAt;
    QVector3D mOrientation;
    QVector3D mVelocity;
    QVector3D mAngularVelocity;
    QVector3D mAcceleration;
    float mMass;
    float mLinearDamping;
    float mAngularDamping;
};

#endif // QTR3DMOTIONBODY_H
