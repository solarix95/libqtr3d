#ifndef QTRPHENTITY_H
#define QTRPHENTITY_H

#include <QObject>
#include <QList>
#include <QVector3D>

class QtrPhEntity : public QObject
{
    Q_OBJECT

public:
    QtrPhEntity(QVector3D pos, QVector3D lookAt, QVector3D up, QObject *parent = nullptr);
    virtual ~QtrPhEntity();

    inline float     collisionRadius() const { return mCollisionRadius; }
    inline QVector3D pos()         const     { return mPos;    }
    inline QVector3D lookAt()      const     { return mLookAt; }
    inline QVector3D up()          const     { return mUp;     }

    virtual bool setPos(const QVector3D &newPos);
    virtual bool process(float ms, float normalizedSpeed);

protected:
    void   setCollisionRadius(float radius);
    virtual void centerForces(QList<QVector3D> &forces) const; // forces to the center of the entity (no momentum)

    virtual void onNewPosition();

private:
    QVector3D mPos;
    QVector3D mLookAt;
    QVector3D mUp;
    QVector3D mMovement;

    float     mCollisionRadius;
};

typedef QList<QtrPhEntity*> QtrPhEntities;
#endif // QTRPHENTITY_H
