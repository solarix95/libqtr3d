#ifndef QTR3DABSTRACTSPACE_H
#define QTR3DABSTRACTSPACE_H

#include <QObject>
#include "qtr3dabstractentity.h"

class Qtr3dForceField;
class Qtr3dAbstractSpace : public QObject
{
    Q_OBJECT
public:
    Qtr3dAbstractSpace(QObject *parent = nullptr);
    virtual ~Qtr3dAbstractSpace();

    virtual void  append(Qtr3dAbstractEntity *entity);
    virtual void  reset();

    Qtr3dForceField              &forceField();
    const Qtr3dAbstractEntities  &entities();

signals:
    void processed();

public slots:
    virtual void process(float ms, float normalizedSpeed);

protected:
    virtual void preProcessing();
    virtual void processEntities(float ms, float normalizedSpeed);
    virtual void postProcessing();

private:
    Qtr3dAbstractEntities  mEntities;
    Qtr3dForceField       *mForceField;
};

#endif // QTR3DABSTRACTSPACE_H
