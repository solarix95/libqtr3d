#ifndef QTR3DSIMULATIONLOOP_H
#define QTR3DSIMULATIONLOOP_H

#include <QObject>
#include <QElapsedTimer>
#include <QTimer>

class Qtr3dSimulationLoop : public QObject
{
    Q_OBJECT
public:
    explicit Qtr3dSimulationLoop(QObject *parent = nullptr);
    virtual ~Qtr3dSimulationLoop();

    void setTargetFps(int fps);
    void setFps(int fps);
    void setTimeScale(float factor);
    void setSpeed(float factor);
    void stop();
    void stepOnce(float realtimeMs = 0.0f);

    bool  isActive() const;
    int   interval() const;
    int   targetFps() const;
    int   currentFps() const;
    float timeScale() const;
    float speed() const;

signals:
    void step(float simulationTimeMs, float normalizedSpeed, int realtimeMs);
    void stepSeconds(float dtSeconds, float timeScale, int realtimeMs);
    void stepDone();

private slots:
    void process();

private:
    void emitStep(float elapsedMs);

    QTimer        mTimer;
    QElapsedTimer mStopWatch;
    float         mTimeScale;
    int           mCurrentFps;
};

#endif // QTR3DSIMULATIONLOOP_H
