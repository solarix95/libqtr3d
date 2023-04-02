#ifndef QTR3DABSTRACTENTITY_H
#define QTR3DABSTRACTENTITY_H

#include <QObject>
#include <QList>
#include "../qtr3dgeometrystate.h"

class Qtr3dAbstractSpace;
class Qtr3dAbstractEntity : public QObject
{
public:
    Qtr3dAbstractEntity(Qtr3dGeometryState &state);
    virtual ~Qtr3dAbstractEntity();

    inline void      setSpace(Qtr3dAbstractSpace *space) { mSpace = space;          }
    inline Qtr3dAbstractSpace *space() const             { return mSpace;           }

    inline QVector3D pos()         const                 { return mPos;             }
    virtual bool     setPos(const QVector3D &newPos);

    virtual bool  process(float ms, float normalizedSpeed) = 0;
    virtual float collisionRadius() const;

protected:
    virtual void onNewPosition();

private:
    QVector3D           mPos;
    Qtr3dAbstractSpace *mSpace;
    Qtr3dGeometryState &mState;
};

typedef QList<Qtr3dAbstractEntity*> Qtr3dAbstractEntities;

#endif // QTR3DABSTRACTENTITY_H
