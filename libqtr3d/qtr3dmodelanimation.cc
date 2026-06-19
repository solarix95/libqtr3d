#include <QDebug>
#include "qtr3dmodelanimation.h"

//-------------------------------------------------------------------------------------------------
Qtr3dModelAnimation::Qtr3dModelAnimation(const QString &name, float duration, int ticksPerSec)
    : mName(name)
    , mDuration(duration)
    , mTicksPerSec(ticksPerSec)
{
}

//-------------------------------------------------------------------------------------------------
QString Qtr3dModelAnimation::name() const
{
    return mName;
}

//-------------------------------------------------------------------------------------------------
float Qtr3dModelAnimation::duration() const
{
    return mDuration;
}

//-------------------------------------------------------------------------------------------------
int Qtr3dModelAnimation::ticksPerSec() const
{
    return mTicksPerSec;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dModelAnimation::addChannel(const Channel &channel)
{
    if (channel.nodeName.isEmpty() || mChannels.contains(channel.nodeName)) {
        qWarning() << QString("Qtr3dModelAnimation: invalid channel: '%1'").arg(channel.nodeName);
        return;
    }
    mChannels[channel.nodeName] = channel;
}

//-------------------------------------------------------------------------------------------------
QStringList Qtr3dModelAnimation::animatedNodes() const
{
    return mChannels.keys();
}

//-------------------------------------------------------------------------------------------------
static Qtr3dModelAnimation::PositionKey positionKeyAt(const QList<Qtr3dModelAnimation::PositionKey> &keys, float time)
{
    if (keys.isEmpty())
        return Qtr3dModelAnimation::PositionKey();
    if (keys.count() == 1 || time <= keys.first().mTime)
        return keys.first();
    if (time >= keys.last().mTime)
        return keys.last();

    for (int i=1; i<keys.count(); ++i) {
        if (time <= keys[i].mTime) {
            const auto &prev = keys[i-1];
            const auto &next = keys[i];
            const float delta = next.mTime - prev.mTime;
            if (delta <= 0.0f)
                return next;
            const float factor = (time - prev.mTime) / delta;
            return Qtr3dModelAnimation::PositionKey(time, prev.mValue + (next.mValue - prev.mValue) * factor);
        }
    }
    return keys.last();
}

static Qtr3dModelAnimation::RotationKey rotationKeyAt(const QList<Qtr3dModelAnimation::RotationKey> &keys, float time)
{
    if (keys.isEmpty())
        return Qtr3dModelAnimation::RotationKey();
    if (keys.count() == 1 || time <= keys.first().mTime)
        return keys.first();
    if (time >= keys.last().mTime)
        return keys.last();

    for (int i=1; i<keys.count(); ++i) {
        if (time <= keys[i].mTime) {
            const auto &prev = keys[i-1];
            const auto &next = keys[i];
            const float delta = next.mTime - prev.mTime;
            if (delta <= 0.0f)
                return next;
            const float factor = (time - prev.mTime) / delta;
            return Qtr3dModelAnimation::RotationKey(time, QQuaternion::nlerp(prev.mValue, next.mValue, factor));
        }
    }
    return keys.last();
}

static Qtr3dModelAnimation::ScaleKey scaleKeyAt(const QList<Qtr3dModelAnimation::ScaleKey> &keys, float time)
{
    if (keys.isEmpty())
        return Qtr3dModelAnimation::ScaleKey();
    if (keys.count() == 1 || time <= keys.first().mTime)
        return keys.first();
    if (time >= keys.last().mTime)
        return keys.last();

    for (int i=1; i<keys.count(); ++i) {
        if (time <= keys[i].mTime) {
            const auto &prev = keys[i-1];
            const auto &next = keys[i];
            const float delta = next.mTime - prev.mTime;
            if (delta <= 0.0f)
                return next;
            const float factor = (time - prev.mTime) / delta;
            return Qtr3dModelAnimation::ScaleKey(time, prev.mValue + (next.mValue - prev.mValue) * factor);
        }
    }
    return keys.last();
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dModelAnimation::tranformNode(const QString &nodeName, float time, QMatrix4x4 &nodeTransform) const
{
    if (!mChannels.contains(nodeName))
        return false;

    if (time > mDuration)
        time = mDuration;
    if (time < 0.0f)
        time = 0.0f;

    const auto &channel = mChannels[nodeName];
    const PositionKey position = positionKeyAt(channel.mPositionKeys, time);
    const RotationKey rotation = rotationKeyAt(channel.mRotationKeys, time);
    const ScaleKey scale = scaleKeyAt(channel.mScaleKeys, time);

    if (!position.isValid() && !rotation.isValid() && !scale.isValid())
        return false;

    if (position.isValid())
        nodeTransform.translate(position.mValue);
    if (rotation.isValid())
        nodeTransform.rotate(rotation.mValue.normalized());
    if (scale.isValid())
        nodeTransform.scale(scale.mValue);

    return true;
}
