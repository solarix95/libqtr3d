#include "qtr3dabstractentity.h"
#include "qtr3dabstractspace.h"
#include "qtr3dforcefield.h"

//-------------------------------------------------------------------------------------------------
Qtr3dAbstractSpace::Qtr3dAbstractSpace(QObject *parent)
 : QObject(parent)
 , mForceField(nullptr)
{
}

//-------------------------------------------------------------------------------------------------
Qtr3dAbstractSpace::~Qtr3dAbstractSpace() = default;


//-------------------------------------------------------------------------------------------------
void Qtr3dAbstractSpace::append(Qtr3dAbstractEntity *entity)
{
    mEntities << entity;
    connect(entity, &Qtr3dAbstractEntity::destroyed, this, [this,entity ](){
        mEntities.removeOne(entity);
    });
    entity->setSpace(this);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dAbstractSpace::reset()
{
}

//-------------------------------------------------------------------------------------------------
Qtr3dForceField &Qtr3dAbstractSpace::forceField()
{
    if (!mForceField)
        mForceField = new Qtr3dForceField(this);
    return *mForceField;
}

//-------------------------------------------------------------------------------------------------
const Qtr3dAbstractEntities &Qtr3dAbstractSpace::entities()
{
    return mEntities;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dAbstractSpace::process(float ms, float normalizedSpeed)
{
    preProcessing();
    processEntities(ms,normalizedSpeed);
    postProcessing();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dAbstractSpace::preProcessing()
{
}

//-------------------------------------------------------------------------------------------------
void Qtr3dAbstractSpace::processEntities(float ms, float normalizedSpeed)
{
    Qtr3dAbstractEntities movedEntities;
    for (auto *e: mEntities) {
        if (e->process(ms, normalizedSpeed))
            movedEntities << e;
    }
   // processDynamicEntities(movedEntities, ms, normalizedSpeed);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dAbstractSpace::postProcessing()
{
}
