#include "qtr3dlightsource.h"

//------------------------------------------------------------------------------------------------
Qtr3dLightSource::Qtr3dLightSource(QObject *parent)
    : QObject(parent)
{
}

//------------------------------------------------------------------------------------------------
Qtr3dLightSource::Qtr3dLightSource(const QVector3D &p, QColor color, QObject *parent)
    : QObject(parent)
    , mPos(p)
    , mColor(color)
{
}

Qtr3dLightSource::~Qtr3dLightSource() = default;

//------------------------------------------------------------------------------------------------
void Qtr3dLightSource::setPos(const QVector3D &p)
{
    mPos = p;
}

//------------------------------------------------------------------------------------------------
QVector3D Qtr3dLightSource::pos() const
{
    return mPos;
}

//------------------------------------------------------------------------------------------------
void Qtr3dLightSource::setColor(QColor c)
{
    mColor = c;
}

//------------------------------------------------------------------------------------------------
QColor Qtr3dLightSource::color() const
{
    return mColor;
}

//------------------------------------------------------------------------------------------------
QVector3D Qtr3dLightSource::ambientColor() const
{
    return QVector3D(mColor.redF()   * mAmbientFactor.x(),
                     mColor.greenF() * mAmbientFactor.y(),
                     mColor.blueF()  * mAmbientFactor.z());
}

//------------------------------------------------------------------------------------------------
void Qtr3dLightSource::setStrength(float ambient, float diffuse, float specular)
{
    mAmbientFactor  = QVector3D(ambient,ambient,ambient);
    mDiffuseFactor  = QVector3D(diffuse,diffuse,diffuse);
    mSpecularFactor = QVector3D(specular,specular,specular);
}

//------------------------------------------------------------------------------------------------
void Qtr3dLightSource::setAmbientStrength(const QVector3D &ambient)
{
    mAmbientFactor = ambient;
}

//------------------------------------------------------------------------------------------------
void Qtr3dLightSource::setDiffuseStrength(const QVector3D &diffuse)
{
    mDiffuseFactor = diffuse;
}

//------------------------------------------------------------------------------------------------
void Qtr3dLightSource::setSpecularStrength(const QVector3D &specular)
{
    mSpecularFactor = specular;
}

//------------------------------------------------------------------------------------------------
QVector3D Qtr3dLightSource::strengthAmbient() const
{
    return mAmbientFactor;
}

//------------------------------------------------------------------------------------------------
QVector3D Qtr3dLightSource::strengthDiffuse() const
{
    return mDiffuseFactor;
}

//------------------------------------------------------------------------------------------------
QVector3D Qtr3dLightSource::strengthSpecular() const
{
    return mSpecularFactor;
}

