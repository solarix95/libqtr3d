#ifndef QTRPHSPACE_H
#define QTRPHSPACE_H

#include <QObject>
#include "qtrphentity.h"

class Qtr3dForceField;

class QtrPhSpace : public QObject
{
public:
    QtrPhSpace(QObject *parent = nullptr);
    virtual ~QtrPhSpace();

    void             append(QtrPhEntity *entity);
    void             setForceField(Qtr3dForceField *field);
    Qtr3dForceField *forceField();
    void             reset();

public slots:
    void process(float ms, float normalizedSpeed);

protected:
    virtual void preProcessing();
    virtual void processEntities(QtrPhEntities &entities, float ms, float normalizedSpeed);
    virtual void processDynamicEntities(QtrPhEntities &entities, float ms, float normalizedSpeed);
    virtual void postProcessing();

private:
   QtrPhEntities    mEntities;
   Qtr3dForceField *mForceField;
};

#endif // QTRPHSPACE_H
