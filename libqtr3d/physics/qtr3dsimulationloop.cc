#include "qtr3dsimulationloop.h"

//-------------------------------------------------------------------------------------------------
Qtr3dSimulationLoop::Qtr3dSimulationLoop(QObject *parent)
    : QObject(parent)
    , mTimeScale(1.0f)
    , mCurrentFps(0)
{
    connect(&mTimer, &QTimer::timeout, this, &Qtr3dSimulationLoop::process);
}

//-------------------------------------------------------------------------------------------------
Qtr3dSimulationLoop::~Qtr3dSimulationLoop() = default;

//-------------------------------------------------------------------------------------------------
void Qtr3dSimulationLoop::setTargetFps(int fps)
{
    mTimer.stop();
    mStopWatch.invalidate();

    if (fps <= 0)
        return;

    mTimer.start(1000/fps);
    mCurrentFps = 0;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dSimulationLoop::setFps(int fps)
{
    setTargetFps(fps);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dSimulationLoop::setTimeScale(float factor)
{
    mTimeScale = factor;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dSimulationLoop::setSpeed(float factor)
{
    setTimeScale(factor);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dSimulationLoop::stop()
{
    mTimer.stop();
    mStopWatch.invalidate();
    mCurrentFps = 0;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dSimulationLoop::stepOnce(float realtimeMs)
{
    if (realtimeMs <= 0.0f)
        realtimeMs = interval() > 0 ? float(interval()) : 16.6667f;
    emitStep(realtimeMs);
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dSimulationLoop::isActive() const
{
    return mTimer.isActive();
}

//-------------------------------------------------------------------------------------------------
int Qtr3dSimulationLoop::interval() const
{
    return mTimer.interval();
}

//-------------------------------------------------------------------------------------------------
int Qtr3dSimulationLoop::targetFps() const
{
    if (mTimer.interval() <= 0)
        return 0;
    return 1000/mTimer.interval();
}

//-------------------------------------------------------------------------------------------------
int Qtr3dSimulationLoop::currentFps() const
{
    return mCurrentFps;
}

//-------------------------------------------------------------------------------------------------
float Qtr3dSimulationLoop::timeScale() const
{
    return mTimeScale;
}

//-------------------------------------------------------------------------------------------------
float Qtr3dSimulationLoop::speed() const
{
    return timeScale();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dSimulationLoop::process()
{
    if (!mStopWatch.isValid()) {
        mStopWatch.start();
        return;
    }

    const int elapsedMs = mStopWatch.elapsed();
    if (elapsedMs <= 0)
        return;

    mStopWatch.start();
    emitStep(float(elapsedMs));
}

//-------------------------------------------------------------------------------------------------
void Qtr3dSimulationLoop::emitStep(float elapsedMs)
{
    const int realMs = int(elapsedMs);
    const float intervalMs = interval() > 0 ? float(interval()) : elapsedMs;
    const float normalizedSpeed = intervalMs > 0.0f ? mTimeScale * elapsedMs / intervalMs : mTimeScale;
    const float simulationMs = elapsedMs * mTimeScale;

    mCurrentFps = realMs > 0 ? 1000/realMs : 0;
    emit step(simulationMs, normalizedSpeed, realMs);
    emit stepSeconds(simulationMs/1000.0f, mTimeScale, realMs);
    emit stepDone();
}
