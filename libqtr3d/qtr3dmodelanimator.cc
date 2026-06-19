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
bool Qtr3dModelAnimator::transform(const QString &nodeName, QMatrix4x4 &nodeTransform) const
{
    if (nodeName.isEmpty() || !mIsEnabled)
        return false;
    return mAnimation->tranformNode(nodeName,locicalAnimationTime(), nodeTransform);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dModelAnimator::setTicksPerSecond(int tps)
{
    mOverrideTickesPerSecond = tps;
}

//-------------------------------------------------------------------------------------------------
float Qtr3dModelAnimator::locicalAnimationTime() const
{
    if (!mAnimationTimer.isValid())
        return 0.0f;

    int tps = mOverrideTickesPerSecond > 0 ? mOverrideTickesPerSecond : mAnimation->ticksPerSec();
    if (tps <= 0)
        tps = 1;

    const float curTimeSecs = float(mAnimationTimer.elapsed()) / 1000.0f;
    float curTimeTicks = curTimeSecs * float(tps);
    const float duration = mAnimation->duration();

    if (mLoop && duration > 0.0f)
        curTimeTicks = std::fmod(curTimeTicks, duration);
    else if (duration > 0.0f && curTimeTicks > duration)
        curTimeTicks = duration;

    return curTimeTicks;
}
