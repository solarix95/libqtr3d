#ifndef QTR3DSIMULATIONWORLD_H
#define QTR3DSIMULATIONWORLD_H

#include <QObject>
#include "qtr3dactor.h"

class Qtr3dForceField;

class Qtr3dSimulationWorld : public QObject
{
    Q_OBJECT
public:
    explicit Qtr3dSimulationWorld(QObject *parent = nullptr);
    virtual ~Qtr3dSimulationWorld();

    virtual void addActor(Qtr3dActor *actor);
    virtual void removeActor(Qtr3dActor *actor);
    virtual void append(Qtr3dActor *actor);
    virtual void reset();

    Qtr3dForceField &forceField();
    const Qtr3dActors &actors() const;
    const Qtr3dActors &entities() const;

signals:
    void processed();

public slots:
    virtual void process(float simulationTimeMs, float normalizedSpeed);
    virtual void update(float dtSeconds);

protected:
    virtual void preUpdate(float dtSeconds);
    virtual void updateActors(float dtSeconds);
    virtual void postUpdate(float dtSeconds);

private:
    Qtr3dActors mActors;
    Qtr3dForceField *mForceField;
};

#endif // QTR3DSIMULATIONWORLD_H
