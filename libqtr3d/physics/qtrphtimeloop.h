#ifndef QTRPHTIMELOOP_H
#define QTRPHTIMELOOP_H

#include <QTimer>
#include <QTime>
#include <QObject>

class QtrPhTimeLoop : public QObject
{
    Q_OBJECT
public:
    QtrPhTimeLoop();

    void init(int fps);
    void setSpeed(int procent);

signals:
    void step(float ms);

private slots:
    void process();

private:
    QTimer mTimer;
    QTime  mStopWatch;
    int    mSpeed;
};

#endif // QTRPHTIMELOOP_H
