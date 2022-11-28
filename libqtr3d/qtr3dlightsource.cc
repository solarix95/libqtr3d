#include "qtr3dlightsource.h"

//------------------------------------------------------------------------------------------------
Qtr3dLightSource::Qtr3dLightSource(QObject *parent)
    : QObject(parent)
    , mPos({0,0,0})
    , mColor(Qt::white)
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
QVector3D Qtr3dLightSource::colorf() const
{
    return QVector3D(mColor.redF(), mColor.greenF(), mColor.blueF());
}

//------------------------------------------------------------------------------------------------
QVector3D Qtr3dLightSource::ambientColor() const
{
    return QVector3D(mColor.redF()   * mAmbientFactor.x(),
                     mColor.greenF() * mAmbientFactor.y(),
                     mColor.blueF()  * mAmbientFactor.z());
}

//------------------------------------------------------------------------------------------------
void Qtr3dLightSource::setAmbientStrength(float kAmbient)
{
    mAmbientFactor = QVector3D(kAmbient,kAmbient,kAmbient);
}

//------------------------------------------------------------------------------------------------
void Qtr3dLightSource::setAmbientStrength(const QVector3D &ambient)
{
    mAmbientFactor = ambient;
}

//------------------------------------------------------------------------------------------------
QVector3D Qtr3dLightSource::strengthAmbient() const
{
    return mAmbientFactor;
}

