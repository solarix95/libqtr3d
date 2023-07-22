#include <QDebug>
#include "qtr3dhighmap.h"

//-------------------------------------------------------------------------------------------------
Qtr3dHighmap::Qtr3dHighmap()
{
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dHighmap::fromImage(const QImage &highmap, const QVector3D &scale)
{
    if ((highmap.width() * highmap.height()) <= 0)
        return false;

    int w  = highmap.width();
    int h  = highmap.height();

    mGeometry  = QSize(w,h);
    mDimension = QVector3D(w * scale.x(), 0, h * scale.z());
    mScale     = scale;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            float fx = scale.x() * ((-w/2.0f) + x);
            float fz = scale.z() * ((-h/2.0f) + y);
            float fy = scale.y() * (qGray(highmap.pixel(x,y))/255.0f);
            QVector3D normal = {0.0f,1.0f,0.0f};
            if ((y > 0) && (x > 0) && (y < (h-1)) && (x < (w-1))) {
                float topHigh    = qGray(highmap.pixel(x,y-1))/255.0f;
                float bottomHigh = qGray(highmap.pixel(x,y+1))/255.0f;

                float rightHigh  = qGray(highmap.pixel(x+1,y))/255.0f;
                float leftHigh   = qGray(highmap.pixel(x-1,y))/255.0f;

                QVector3D horiz = { scale.x()*2, scale.y()*(rightHigh-leftHigh), 0};
                QVector3D verti = { 0,           scale.y()*(bottomHigh-topHigh), scale.z()*2};

                normal = QVector3D::crossProduct(verti,horiz).normalized();
            }

            mVectorField << QVector3D{fx,fy,fz};
            mNormals     << normal;

            if (fy > mDimension.y())
                mDimension.setY(fy);
        }
    }

    return true;
}

//-------------------------------------------------------------------------------------------------
const QSize &Qtr3dHighmap::geometry() const
{
    return mGeometry;
}

//-------------------------------------------------------------------------------------------------
const QVector3D &Qtr3dHighmap::vector(int px, int py) const
{
    Q_ASSERT(px >= 0);
    Q_ASSERT(py >= 0);

    Q_ASSERT(px < mGeometry.width());
    Q_ASSERT(py < mGeometry.height());

    return mVectorField[(py * mGeometry.width()) + px];
}

//-------------------------------------------------------------------------------------------------
const QVector3D &Qtr3dHighmap::normal(int px, int py) const
{
    Q_ASSERT(px >= 0);
    Q_ASSERT(py >= 0);

    Q_ASSERT(px < mGeometry.width());
    Q_ASSERT(py < mGeometry.height());

    return mNormals[(py * mGeometry.width()) + px];
}

//-------------------------------------------------------------------------------------------------
const QVector3D &Qtr3dHighmap::dimension() const
{
    return mDimension;
}

//-------------------------------------------------------------------------------------------------
float Qtr3dHighmap::yHightAt(float x, float z) const
{
    if (mVectorField.isEmpty())
        return 0;

    if (x < mVectorField.first().x())
        return 0;
    if (x > mVectorField.last().x())
        return 0;
    if (z < mVectorField.first().z())
        return 0;
    if (z > mVectorField.last().z())
        return 0;

    int xIndex = (x - mVectorField.first().x())/mScale.x();
    int zIndex = (z - mVectorField.first().z())/mScale.z();

    int topLeftIndex = zIndex * mGeometry.height() + xIndex;
    Q_ASSERT(topLeftIndex < mVectorField.count());

    qDebug() << x << z << mVectorField[topLeftIndex].x() << mVectorField[topLeftIndex].z();
    return mVectorField[topLeftIndex].y();


    // return mDimension.y();
}


