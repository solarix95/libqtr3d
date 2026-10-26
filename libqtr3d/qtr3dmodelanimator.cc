#include <cmath>
#include "qtr3dmodelanimator.h"
#include "qtr3dmodelanimation.h"

//-------------------------------------------------------------------------------------------------
Qtr3dModelAnimator::Qtr3dModelAnimator(Qtr3dModelAnimation *animation)
    : QObject()
    , mAnimation(animation)
    , mOverrideTickesPerSecond(-1)
    , mIsEnabled(false)
    , mLoop(false)
    , mTransitionDurationTicks(0.0f)
{
    Q_ASSERT(mAnimation);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dModelAnimator::start()
{
    mIsEnabled = true;
    mAnimationTimer.start();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dModelAnimator::stop()
{
    mIsEnabled = false;
    mAnimationTimer.invalidate();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dModelAnimator::setState(bool animationRunning)
{
    if (animationRunning)
        start();
    else
        stop();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dModelAnimator::setEnabled(bool enabled)
{
    mIsEnabled = enabled;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dModelAnimator::setLoop(bool loop)
{
    mLoop = loop;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dModelAnimator::transitionFrom(const Qtr3dModelAnimator *sourceAnimator,
                                        float durationTicks)
{
    mTransitionSourcePose.clear();
    mTransitionDurationTicks = 0.0f;

    if (!sourceAnimator || sourceAnimator == this || !sourceAnimator->mIsEnabled
            || !std::isfinite(durationTicks) || durationTicks <= 0.0f)
        return;

    QStringList nodeNames = mAnimation->animatedNodes();
    for (const QString &nodeName: sourceAnimator->mAnimation->animatedNodes()) {
        if (!nodeNames.contains(nodeName))
            nodeNames << nodeName;
    }

    const float sourceTime = sourceAnimator->logicalAnimationTime();
    for (const QString &nodeName: nodeNames) {
        Qtr3dModelAnimation::NodeTransform sourceTransform;
        sourceAnimator->mAnimation->nodeTransform(nodeName, sourceTime, sourceTransform);
        mTransitionSourcePose.insert(nodeName, sourceTransform);
    }
    mTransitionDurationTicks = durationTicks;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dModelAnimator::transitionActive() const
{
    return mIsEnabled && mTransitionDurationTicks > 0.0f
            && logicalAnimationTime() < mTransitionDurationTicks;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dModelAnimator::transform(const QString &nodeName, QMatrix4x4 &nodeTransform) const
{
    if (nodeName.isEmpty() || !mIsEnabled)
        return false;

    const float animationTime = logicalAnimationTime();
    Qtr3dModelAnimation::NodeTransform targetTransform;
    const bool hasTarget = mAnimation->nodeTransform(nodeName, animationTime, targetTransform);

    const auto source = mTransitionSourcePose.constFind(nodeName);
    if (source != mTransitionSourcePose.constEnd()
            && mTransitionDurationTicks > 0.0f
            && animationTime < mTransitionDurationTicks) {
        const float factor = qBound(0.0f, animationTime/mTransitionDurationTicks, 1.0f);
        const Qtr3dModelAnimation::NodeTransform &sourceTransform = source.value();
        targetTransform.position = sourceTransform.position
                + factor*(targetTransform.position-sourceTransform.position);
        targetTransform.rotation = QQuaternion::nlerp(sourceTransform.rotation,
                                                       targetTransform.rotation,
                                                       factor).normalized();
        targetTransform.scale = sourceTransform.scale
                + factor*(targetTransform.scale-sourceTransform.scale);

        nodeTransform.translate(targetTransform.position);
        nodeTransform.rotate(targetTransform.rotation);
        nodeTransform.scale(targetTransform.scale);
        return true;
    }

    if (!hasTarget)
        return false;
    nodeTransform.translate(targetTransform.position);
    nodeTransform.rotate(targetTransform.rotation);
    nodeTransform.scale(targetTransform.scale);
    return true;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dModelAnimator::setTicksPerSecond(int tps)
{
    mOverrideTickesPerSecond = tps;
}

//-------------------------------------------------------------------------------------------------
float Qtr3dModelAnimator::animationTime() const
{
    return logicalAnimationTime();
}

//-------------------------------------------------------------------------------------------------
float Qtr3dModelAnimator::logicalAnimationTime() const
{
    if (!mAnimationTimer.isValid())
        return 0.0f;

    int tps = mOverrideTickesPerSecond > 0 ? mOverrideTickesPerSecond : mAnimation->ticksPerSec();
    if (tps <= 0)
        tps = 1;

    const float curTimeSecs = float(mAnimationTimer.elapsed()) / 1000.0f;
    float curTimeTicks = curTimeSecs * float(tps);
    const float duration = mAnimation->duration();

    if (mLoop && duration > 0.0f && curTimeTicks >= duration) {
        const float loopStart = mAnimation->loopStart();
        const float loopDuration = duration - loopStart;
        if (loopDuration > 0.0f)
            curTimeTicks = loopStart + std::fmod(curTimeTicks - duration, loopDuration);
        else
            curTimeTicks = duration;
    }
    else if (duration > 0.0f && curTimeTicks > duration)
        curTimeTicks = duration;

    return curTimeTicks;
}
