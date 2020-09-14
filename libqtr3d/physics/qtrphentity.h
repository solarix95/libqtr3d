#ifndef QTRPHENTITY_H
#define QTRPHENTITY_H

#include <QPointF>

class QtrPhEntity
{
public:
    QtrPhEntity();

    inline float collisionRadius() const { return mCollisionRadius; }
    inline QPointF pos();

protected:
    void setCollisionRadius(float radius);

private:
    float mCollisionRadius;
};

#endif // QTRPHENTITY_H
