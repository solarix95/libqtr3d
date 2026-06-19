#ifndef QTR3DACTOR_H
#define QTR3DACTOR_H

#include <QObject>
#include <QList>
#include <QVector3D>
#include "../qtr3dgeometrystate.h"

class Qtr3dSimulationWorld;

class Qtr3dActor : public QObject
{
    Q_OBJECT
public:
    explicit Qtr3dActor(Qtr3dGeometryState *state = nullptr, QObject *parent = nullptr);
    virtual ~Qtr3dActor();

    void setState(Qtr3dGeometryState *state);
    Qtr3dGeometryState *state() const;

    void setWorld(Qtr3dSimulationWorld *world);
    Qtr3dSimulationWorld *world() const;

    QVector3D pos() const;
    virtual bool setPos(const QVector3D &newPos);

    QVector3D rotation() const;
    QVector3D rot() const;
    virtual bool setRotation(const QVector3D &rotation);

    QVector3D scale() const;
    virtual bool setScale(const QVector3D &scale);
    virtual bool setScale(float xyzScale);

    virtual bool update(float dtSeconds);
    virtual bool process(float simulationTimeMs, float normalizedSpeed);
    virtual float collisionRadius() const;

signals:
    void positionChanged(const QVector3D &pos);
    void rotationChanged(const QVector3D &rotation);
    void scaleChanged(const QVector3D &scale);
    void stateChanged(Qtr3dGeometryState *state);

protected:
    virtual void onNewPosition();
    virtual void onNewRotation();
    virtual void onNewScale();
    virtual void onNewState();

private:
    QVector3D mPos;
    QVector3D mRotation;
    QVector3D mScale;
    Qtr3dSimulationWorld *mWorld;
    Qtr3dGeometryState *mState;
};

typedef QList<Qtr3dActor*> Qtr3dActors;

#endif // QTR3DACTOR_H
