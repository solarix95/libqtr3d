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
QMatrix4x4 Qtr3dModelAnimation::tranformNode(const QString &nodeName, float time) const
{
    QMatrix4x4 ret;
    ret.setToIdentity();

    if (!mChannels.contains(nodeName))
        return ret;

    if (time > mDuration)
        time = mDuration;

    // TODO
    // Select keyFrame 1
    // Select keyFrame 2
    // interpolate
    return ret;
}

//-------------------------------------------------------------------------------------------------
QMatrix4x4 Qtr3dModelAnimation::tranformNode(const QString &nodeName, int keyFrame) const
{
    QMatrix4x4 ret;
    ret.setToIdentity();

    if (!mChannels.contains(nodeName))
        return ret;

    if (keyFrame < 0)
        return ret;

    const auto &channel = mChannels[nodeName];
    return transformChannel(channel,keyFrame,ret);
}

//-------------------------------------------------------------------------------------------------
const QMatrix4x4 &Qtr3dModelAnimation::transformChannel(const Channel &channel, int keyFrame, QMatrix4x4 &matrix) const
{
    return matrix;
}
