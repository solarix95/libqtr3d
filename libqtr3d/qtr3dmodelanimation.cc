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
bool Qtr3dModelAnimation::tranformNode(const QString &nodeName, float time, QMatrix4x4 &nodeTransform) const
{
    if (!mChannels.contains(nodeName))
        return false;

    if (time > mDuration)
        time = mDuration;

    const auto &channel = mChannels[nodeName];

    // Schritt 1: Interpolation der Position
    int positionIndex = -1;
    for (int j = 0; j < channel.mPositionKeys.count(); ++j) {
        if (time <= channel.mPositionKeys[j].mTime) {
            positionIndex = j;
            break;
        }
    }

    QVector3D position;
    if (channel.mPositionKeys.count() == 1 || positionIndex == 0) { // Sonderfall: vor dem ersten KeyFrame
        position = channel.mPositionKeys[0].mValue;
    } else if (positionIndex > 0) {
        QVector3D startState = channel.mPositionKeys[positionIndex - 1].mValue;
        QVector3D endState   = channel.mPositionKeys[positionIndex].mValue;
        float deltaTime      = channel.mPositionKeys[positionIndex].mTime - channel.mPositionKeys[positionIndex - 1].mTime;
        float factor         = (time - channel.mPositionKeys[positionIndex - 1].mTime) / deltaTime;
        position             = startState + (endState - startState) * factor;
    }

    // Schritt 2: Interpolation der Rotation
    int rotationIndex = -1;
    for (int j = 0; j < channel.mRotationKeys.count(); ++j) {
        if (time <= channel.mRotationKeys[j].mTime) {
            rotationIndex = j;
            break;
        }
    }
    QQuaternion rotation;
    if (channel.mRotationKeys.count() == 1 || rotationIndex == 0) { // Sonderfall: vor dem ersten KeyFrame
        rotation = channel.mRotationKeys[0].mValue;
    } else if (rotationIndex > 0) {
        QQuaternion startState = channel.mRotationKeys[rotationIndex - 1].mValue;
        QQuaternion endState   = channel.mRotationKeys[rotationIndex].mValue;
        float deltaTime        = channel.mRotationKeys[rotationIndex].mTime - channel.mRotationKeys[rotationIndex - 1].mTime;
        float factor           = (time - channel.mRotationKeys[rotationIndex - 1].mTime) / deltaTime;
        rotation = QQuaternion::nlerp(startState,endState,factor);
    }

    // Schritt 3: Interpolation der Skalierung
    int scaleIndex = -1;
    for (int j = 0; j < channel.mScaleKeys.count(); ++j) {
        if (time <= channel.mScaleKeys[j].mTime) {
            scaleIndex = j;
            break;
        }
    }
    QVector3D scale(1,1,1);
    if (channel.mScaleKeys.count() == 1 || scaleIndex == 0) { // Sonderfall: vor dem ersten KeyFrame
        scale = channel.mScaleKeys[0].mValue;
    } else if (scaleIndex > 0) {
        QVector3D startState = channel.mScaleKeys[scaleIndex - 1].mValue;
        QVector3D endState   = channel.mScaleKeys[scaleIndex].mValue;
        float deltaTime      = channel.mScaleKeys[scaleIndex].mTime - channel.mScaleKeys[scaleIndex - 1].mTime;
        float factor         = (time - channel.mScaleKeys[scaleIndex - 1].mTime) / deltaTime;
        scale                = startState + (endState - startState) * factor;
    }
    QMatrix4x4 temp3;
    temp3.scale(scale);

    // nodeTransform = QMatrix4x4();
    nodeTransform.translate(position);
    nodeTransform.rotate(rotation);
    nodeTransform.scale(scale);

    return true;
}
