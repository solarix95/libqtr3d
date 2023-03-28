#include "qtr3dfpsloop.h"

//-------------------------------------------------------------------------------------------------
Qtr3dFpsLoop::Qtr3dFpsLoop(QObject *parent)
 : QObject(parent)
 , mSpeed(100)
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
}

//-------------------------------------------------------------------------------------------------
void Qtr3dFpsLoop::setSpeed(int procent)
{
    mSpeed = procent;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dFpsLoop::process()
{
    if (!mStopWatch.isValid()) { // skip first interval
        mStopWatch.start();
        return;
    }
    int   elapsedMs       = mStopWatch.elapsed();
    float normalizedSpeed = elapsedMs/mTimer.interval();

    mStopWatch.start();
    emit step(elapsedMs*(mSpeed/100), mSpeed*normalizedSpeed);
    emit stepDone();
}
