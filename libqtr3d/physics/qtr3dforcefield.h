#ifndef QTR3DFORCEFIELD_H
#define QTR3DFORCEFIELD_H

#include <QObject>
#include <QVector3D>
#include <QList>

class Qtr3dForceField : public QObject
{
public:
    Qtr3dForceField(QObject *parent = nullptr);

    void setConstantForces(const QList<QVector3D> &forces);
    void setConstantForce(const QVector3D &force);
    virtual void forceVector(const QVector3D &pos, QList<QVector3D> &forces);

private:
    QList<QVector3D> mConstantForces;
};

#endif // QTR3DFORCEFIELD_H
