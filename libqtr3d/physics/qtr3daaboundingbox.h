#ifndef QTR3DAABOUNDINGBOX_H
#define QTR3DAABOUNDINGBOX_H

#include <QVector3D>

class Qtr3dAaBoundingBox
{
public:
    Qtr3dAaBoundingBox();
    Qtr3dAaBoundingBox(const QVector3D &p1, const QVector3D &p2);

    void      setPoints(const QVector3D &p1, const QVector3D &p2);
    QVector3D bottomCenter() const;

    inline const QVector3D &min() const { return mMin; }
    inline const QVector3D &max() const { return mMax; }

private:
    QVector3D mMin;
    QVector3D mMax;
};

#endif // QTR3DAABOUNDINGBOX_H
