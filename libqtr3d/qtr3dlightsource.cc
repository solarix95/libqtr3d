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


