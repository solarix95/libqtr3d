#ifndef QTR3DHIGHMAP_H
#define QTR3DHIGHMAP_H

#include <QObject>
#include <QImage>
#include <QList>
#include <QVector3D>
#include <QVector2D>
#include <QSize>

class Qtr3dHighmap
{
public:
    Qtr3dHighmap();

    bool fromImage(const QImage &highmap, const QVector3D &scale = {1,1,1});

    // geometry
    const QSize     &geometry() const;
    const QVector3D &vector(int px, int py) const;
    const QVector3D &normal(int px, int py) const;

    // virtual dimension
    const QVector3D &dimension() const;
    float            yHightAt(float x, float z) const;

private:
    QSize            mGeometry;
    QVector3D        mDimension;
    QVector3D        mScale;
    QList<QVector3D> mVectorField;
    QList<QVector3D> mNormals;
};

#endif // QTR3DHIGHMAP_H
