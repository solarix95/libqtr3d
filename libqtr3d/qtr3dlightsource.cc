#include "qtr3dlightsource.h"
#include <QtGlobal>

//------------------------------------------------------------------------------------------------
Qtr3dLightSource::Qtr3dLightSource(QObject *parent)
    : QObject(parent)
    , mPos(0,0,0)
    , mColor(Qt::white)
    , mAmbientFactor(0,0,0)
    , mIntensity(1.0f)
    , mRange(-1.0f)
    , mEnabled(true)
{
}

//------------------------------------------------------------------------------------------------
Qtr3dLightSource::Qtr3dLightSource(const QVector3D &p, QColor color, QObject *parent)
    : QObject(parent)
    , mPos(p)
    , mColor(color)
    , mAmbientFactor(0,0,0)
    , mIntensity(1.0f)
    , mRange(-1.0f)
    , mEnabled(true)
{
}

Qtr3dLightSource::~Qtr3dLightSource() = default;

//------------------------------------------------------------------------------------------------
void Qtr3dLightSource::setPos(const Qtr3dDblVector3D &p)
{
    if (mPos == p)
        return;
    mPos = p;
    emit changed(this);
}

//------------------------------------------------------------------------------------------------
Qtr3dDblVector3D Qtr3dLightSource::pos() const
{
    return mPos;
}

//------------------------------------------------------------------------------------------------
void Qtr3dLightSource::setColor(QColor c)
{
    if (mColor == c)
        return;
    mColor = c;
    emit changed(this);
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
    setAmbientStrength(QVector3D(kAmbient,kAmbient,kAmbient));
}

//------------------------------------------------------------------------------------------------
void Qtr3dLightSource::setAmbientStrength(const QVector3D &ambient)
{
    if (mAmbientFactor == ambient)
        return;
    mAmbientFactor = ambient;
    emit changed(this);
}

//------------------------------------------------------------------------------------------------
QVector3D Qtr3dLightSource::strengthAmbient() const
{
    return mAmbientFactor;
}


//------------------------------------------------------------------------------------------------
float Qtr3dLightSource::intensity() const { return mIntensity; }

void Qtr3dLightSource::setIntensity(float intensity)
{
    intensity = qMax(0.0f, intensity);
    if (qFuzzyCompare(mIntensity, intensity)) return;
    mIntensity = intensity;
    emit changed(this);
}

float Qtr3dLightSource::range() const { return mRange; }

void Qtr3dLightSource::setRange(float range)
{
    if (qFuzzyCompare(mRange, range)) return;
    mRange = range;
    emit changed(this);
}

bool Qtr3dLightSource::enabled() const { return mEnabled; }

void Qtr3dLightSource::setEnabled(bool enabled)
{
    if (mEnabled == enabled) return;
    mEnabled = enabled;
    emit changed(this);
}

float Qtr3dLightSource::influenceAt(const Qtr3dDblVector3D &point, float objectRadius) const
{
    if (!mEnabled || mIntensity <= 0.0f) return 0.0f;
    const float distance = qMax(0.0f, float(mPos.distanceToPoint(point)) - qMax(0.0f, objectRadius));
    if (mRange >= 0.0f && distance > mRange) return 0.0f;

    float influence = mIntensity / (1.0f + distance * distance);
    if (mRange > 0.0f) {
        const float remaining = qMax(0.0f, 1.0f - distance / mRange);
        influence *= remaining * remaining;
    }
    return influence;
}
