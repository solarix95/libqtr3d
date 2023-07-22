#ifndef QTR3DSTANDARDENTITY_H
#define QTR3DSTANDARDENTITY_H

#include <QVector3D>
#include "qtr3dabstractentity.h"
#include "../qtr3dgeometrystate.h"

class Qtr3dStandardEntity : public Qtr3dAbstractEntity
{
    Q_OBJECT
public:
    Qtr3dStandardEntity(Qtr3dGeometryState *state, QVector3D pos, QVector3D lookAt = {0,0,0}, QVector3D orientation = {0,1,0});
    virtual ~Qtr3dStandardEntity();

    inline QVector3D lookAt()      const         { return mLookAt;          }
    inline QVector3D orientation() const         { return mOrientation;     }
    inline QVector3D movement()    const         { return mMovement;        }

    inline void      setLookAt(const QVector3D &dir)              { if (mLookAt == dir)      return; mLookAt=dir;      emit lookAtChanged(dir);      }
    inline void      setOrientation(const QVector3D &dir)         { if (mOrientation == dir) return; mOrientation=dir; emit orientationChanged(dir); }
    inline void      setMovement(const QVector3D &deltaV)         { mMovement = deltaV;          }
    inline void      setAutorotation(const QVector3D &deltaAlpha) { mAutoRotation = deltaAlpha;  }


    void relativeMove(float distance); // fore and back,               changes "position"
    void relativeStrafe(float distance);       // left/right,          changes "position"
    void relativeTurn(float angle);            // plane: "rudder/yaw", changes "lookAt"
    void relativeRoll(float angle);            // plane: "aileron",        changes "orientation"
    void relativePitch(float angle);           // plane: "aileron", "nick",changes "lookAt" + "orientation"

    virtual bool process(float ms, float normalizedSpeed);

signals:
    void lookAtChanged(const QVector3D &newLookAt);
    void orientationChanged(const QVector3D &newOrientation);

protected:
    virtual void centerForces(QList<QVector3D> &forces) const; // forces to the center of the entity (no momentum)

private:
    QVector3D mLookAt;
    QVector3D mOrientation;
    QVector3D mMovement;
    QVector3D mAutoRotation;
};

#endif // QTR3DSTANDARDENTITY_H
