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
    return QMatrix4x4();
}
