#include "qtr3daaboundingbox.h"

Qtr3dAaBoundingBox::Qtr3dAaBoundingBox()
{
}

Qtr3dAaBoundingBox::Qtr3dAaBoundingBox(const QVector3D &p1, const QVector3D &p2)
{
   setPoints(p1,p2);
}

void Qtr3dAaBoundingBox::setPoints(const QVector3D &p1, const QVector3D &p2)
{
    mMin = QVector3D(
                qMin(p1.x(),p2.x()),
                qMin(p1.y(),p2.y()),
                qMin(p1.z(),p2.z()));

    mMax = QVector3D(
                qMax(p1.x(),p2.x()),
                qMax(p1.y(),p2.y()),
                qMax(p1.z(),p2.z()));
}

QVector3D Qtr3dAaBoundingBox::bottomCenter() const
{
    return QVector3D((mMax.x() + mMin.x())/2,
                     (mMin.y()),
                     (mMax.z() + mMin.z())/2);
}
