#ifndef QTR3DSTANDARDENTITY_H
#define QTR3DSTANDARDENTITY_H

#include <QVector3D>
#include "qtr3dabstractentity.h"
#include "../qtr3dgeometrystate.h"

class Qtr3dStandardEntity : public Qtr3dAbstractEntity
{
    Q_OBJECT
public:
    Qtr3dStandardEntity(Qtr3dGeometryState &state, QVector3D pos, QVector3D lookAt = {0,0,0}, QVector3D orientation = {0,1,0});
    virtual ~Qtr3dStandardEntity();

    inline QVector3D lookAt()      const         { return mLookAt;          }
    inline QVector3D orientation() const         { return mOrientation;     }
    inline QVector3D movement()    const         { return mMovement;        }

    inline void      setLookAt(const QVector3D &target)           { if (mLookAt == target) return; mLookAt=target; emit lookAtChanged(mLookAt); }
    inline void      setMovement(const QVector3D &deltaV)         { mMovement = deltaV;          }
    inline void      setAutorotation(const QVector3D &deltaAlpha) { mAutoRotation = deltaAlpha;  }

    virtual bool process(float ms, float normalizedSpeed);

signals:
    void lookAtChanged(const QVector3D &newLookAt);

protected:
    virtual void centerForces(QList<QVector3D> &forces) const; // forces to the center of the entity (no momentum)

private:
    QVector3D mLookAt;
    QVector3D mOrientation;
    QVector3D mMovement;
    QVector3D mAutoRotation;
};

#endif // QTR3DSTANDARDENTITY_H
