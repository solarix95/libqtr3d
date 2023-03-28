#include "qtrphspace.h"
#include "qtr3dforcefield.h"

//-------------------------------------------------------------------------------------------------
QtrPhSpace::QtrPhSpace(QObject *parent)
 : QObject(parent)
 , mForceField(nullptr)
{
}

//-------------------------------------------------------------------------------------------------
QtrPhSpace::~QtrPhSpace()
{
    reset();
}

//-------------------------------------------------------------------------------------------------
void QtrPhSpace::append(QtrPhEntity *entity)
{
    mEntities << entity;
    connect(entity, &QtrPhEntity::destroyed, this, [this,entity ](){
        mEntities.removeOne(entity);
    });
    entity->setSpace(this);
}

//-------------------------------------------------------------------------------------------------
void QtrPhSpace::setForceField(Qtr3dForceField *field)
{
    mForceField = field;
}

//-------------------------------------------------------------------------------------------------
Qtr3dForceField *QtrPhSpace::forceField()
{
    if (!mForceField)
        mForceField = new Qtr3dForceField();
    return mForceField;
}

//-------------------------------------------------------------------------------------------------
void QtrPhSpace::reset()
{
    qDeleteAll(mEntities);
    mEntities.clear();
}

//-------------------------------------------------------------------------------------------------
void QtrPhSpace::process(float ms, float normalizedSpeed)
{
    preProcessing();
    processEntities(mEntities, ms, normalizedSpeed);
    postProcessing();
}

//-------------------------------------------------------------------------------------------------
void QtrPhSpace::preProcessing()
{
}

//-------------------------------------------------------------------------------------------------
void QtrPhSpace::processEntities(QList<QtrPhEntity *> &entities, float ms, float normalizedSpeed)
{
    QtrPhEntities movedEntities;
    for (auto *e: entities) {
        if (e->process(ms, normalizedSpeed))
            movedEntities << e;
    }
    processDynamicEntities(movedEntities, ms, normalizedSpeed);
}

//-------------------------------------------------------------------------------------------------
void QtrPhSpace::processDynamicEntities(QtrPhEntities &entities, float ms, float normalizedSpeed)
{
}

//-------------------------------------------------------------------------------------------------
void QtrPhSpace::postProcessing()
{
}
