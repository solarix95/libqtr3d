#include "qtrphtimeloop.h"

//-------------------------------------------------------------------------------------------------
QtrPhTimeLoop::QtrPhTimeLoop()
{
    connect(&mTimer, &QTimer::timeout, this, &QtrPhTimeLoop::process);
}

//-------------------------------------------------------------------------------------------------
void QtrPhTimeLoop::init(int fps)
{
    Q_ASSERT(fps > 0);
    mStopWatch.invalidate();
    mSpeed     = 100;
    mTimer.start(1000/fps);
}

//-------------------------------------------------------------------------------------------------
void QtrPhTimeLoop::setSpeed(int procent)
{
    mSpeed = procent;
}

//-------------------------------------------------------------------------------------------------
void QtrPhTimeLoop::process()
{
    if (mStopWatch.isValid()) {
        mStopWatch.start();
        return;
    }
    int elapsedMs = mStopWatch.elapsed();
    mStopWatch.start();

    emit step(elapsedMs*(mSpeed/100));
}
