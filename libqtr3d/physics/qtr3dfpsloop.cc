#include <QDebug>
#include "qtr3dfpsloop.h"

//-------------------------------------------------------------------------------------------------
Qtr3dFpsLoop::Qtr3dFpsLoop(QObject *parent)
 : QObject(parent)
 , mSpeed(100)
 , mCurrentFps(0)
{
    connect(&mTimer, &QTimer::timeout, this, &Qtr3dFpsLoop::process);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dFpsLoop::setFps(int fps)
{
    mTimer.stop();
    mStopWatch.invalidate();

    if (fps <= 0)
        return;

    mTimer.start(1000/fps);
    mCurrentFps = 0;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dFpsLoop::setSpeed(int procent)
{
    mSpeed = procent;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dFpsLoop::isActive() const
{
    return mTimer.isActive();
}

//-------------------------------------------------------------------------------------------------
int Qtr3dFpsLoop::interval() const
{
    return mTimer.interval();
}

//-------------------------------------------------------------------------------------------------
int Qtr3dFpsLoop::targetFps() const
{
    if (mTimer.interval() <= 0)
        return 0;
    return 1000/mTimer.interval();
}

//-------------------------------------------------------------------------------------------------
int Qtr3dFpsLoop::currentFps() const
{
    return mCurrentFps;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dFpsLoop::process()
{
    if (!mStopWatch.isValid()) { // skip first interval
        mStopWatch.start();
        return;
    }
    int   elapsedMs       = mStopWatch.elapsed();
    float normalizedSpeed = elapsedMs/float(mTimer.interval());

    if (elapsedMs <= 0)
        return;

    mCurrentFps = 1000/elapsedMs;
    mStopWatch.start();
    emit step(float(elapsedMs)*(mSpeed/100), mSpeed*normalizedSpeed);
    emit stepDone();
}
