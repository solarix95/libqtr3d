#include "qtrphspace.h"

//-------------------------------------------------------------------------------------------------
QtrPhSpace::QtrPhSpace(QObject *parent)
 : QObject(parent)
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
