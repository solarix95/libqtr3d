#include "qtr3dsimulationworld.h"
#include "qtr3dforcefield.h"

//-------------------------------------------------------------------------------------------------
Qtr3dSimulationWorld::Qtr3dSimulationWorld(QObject *parent)
    : QObject(parent)
    , mForceField(nullptr)
{
}

//-------------------------------------------------------------------------------------------------
Qtr3dSimulationWorld::~Qtr3dSimulationWorld() = default;

//-------------------------------------------------------------------------------------------------
void Qtr3dSimulationWorld::addActor(Qtr3dActor *actor)
{
    if (!actor || mActors.contains(actor))
        return;

    mActors << actor;
    connect(actor, &Qtr3dActor::destroyed, this, [this, actor](){
        mActors.removeOne(actor);
    });
    actor->setWorld(this);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dSimulationWorld::removeActor(Qtr3dActor *actor)
{
    if (!actor)
        return;
    if (mActors.removeOne(actor))
        actor->setWorld(nullptr);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dSimulationWorld::append(Qtr3dActor *actor)
{
    addActor(actor);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dSimulationWorld::reset()
{
    for (auto *actor: mActors) {
        if (actor)
            actor->setWorld(nullptr);
    }
    mActors.clear();
}

//-------------------------------------------------------------------------------------------------
Qtr3dForceField &Qtr3dSimulationWorld::forceField()
{
    if (!mForceField)
        mForceField = new Qtr3dForceField(this);
    return *mForceField;
}

//-------------------------------------------------------------------------------------------------
const Qtr3dActors &Qtr3dSimulationWorld::actors() const
{
    return mActors;
}

//-------------------------------------------------------------------------------------------------
const Qtr3dActors &Qtr3dSimulationWorld::entities() const
{
    return mActors;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dSimulationWorld::process(float simulationTimeMs, float normalizedSpeed)
{
    Q_UNUSED(normalizedSpeed);
    update(simulationTimeMs/1000.0f);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dSimulationWorld::update(float dtSeconds)
{
    preUpdate(dtSeconds);
    updateActors(dtSeconds);
    postUpdate(dtSeconds);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dSimulationWorld::preUpdate(float dtSeconds)
{
    Q_UNUSED(dtSeconds);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dSimulationWorld::updateActors(float dtSeconds)
{
    const Qtr3dActors actorsSnapshot = mActors;
    for (auto *actor: actorsSnapshot) {
        if (actor)
            actor->update(dtSeconds);
    }
}

//-------------------------------------------------------------------------------------------------
void Qtr3dSimulationWorld::postUpdate(float dtSeconds)
{
    Q_UNUSED(dtSeconds);
    emit processed();
}
