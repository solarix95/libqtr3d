#ifndef QTR3DFPSLOOP_H
#define QTR3DFPSLOOP_H

#include <QTimer>
#include <QElapsedTimer>
#include <QObject>

class Qtr3dFpsLoop : public QObject
{
    Q_OBJECT
public:
    Qtr3dFpsLoop(QObject *parent = nullptr);

    void setFps(int fps);
    void setSpeed(int procent);

    bool isActive()   const;
    int  interval()   const;
    int  targetFps()  const;
    int  currentFps() const;

signals:
    void step(float ms, float normalizedSpeed);
    void stepDone();

private slots:
    void process();

private:
    QTimer         mTimer;
    QElapsedTimer  mStopWatch;
    int            mSpeed;
    int            mCurrentFps;
};

#endif // QTR3DFPSLOOP_H
