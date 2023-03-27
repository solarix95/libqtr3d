#ifndef QTRPHSPACE_H
#define QTRPHSPACE_H

#include <QObject>
#include "qtrphentity.h"

class QtrPhSpace : public QObject
{
public:
    QtrPhSpace(QObject *parent = nullptr);
    virtual ~QtrPhSpace();

    void append(QtrPhEntity *entity);
    void reset();

public slots:
    void process(float ms, float normalizedSpeed);

protected:
    virtual void preProcessing();
    virtual void processEntities(QtrPhEntities &entities, float ms, float normalizedSpeed);
    virtual void processDynamicEntities(QtrPhEntities &entities, float ms, float normalizedSpeed);
    virtual void postProcessing();

private:
   QtrPhEntities mEntities;
};

#endif // QTRPHSPACE_H
