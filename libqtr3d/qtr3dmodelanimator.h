#ifndef QTR3DMODELANIMATOR_H
#define QTR3DMODELANIMATOR_H

#include <QObject>
#include <QElapsedTimer>
#include <QStringList>
#include <QMatrix4x4>
#include <QVector3D>

class Qtr3dModelAnimation;
class Qtr3dModelAnimator : public QObject
{
public:
    Qtr3dModelAnimator(Qtr3dModelAnimation *animation);

    void       start();
    void       stop();
    void       setState(bool animationRunning);
    void       setEnabled(bool enabled);

    void       setLoop(bool loop);
    bool       transform(const QString &nodeName, QMatrix4x4 &nodeTransform) const;
    void       setTicksPerSecond(int tps);

private:
    float locicalAnimationTime() const;

    Qtr3dModelAnimation *mAnimation;
    QElapsedTimer        mAnimationTimer;
    int                  mOverrideTickesPerSecond;
    bool                 mIsEnabled;
    bool                 mLoop;
};

#endif // QTR3DMODELANIMATOR_H
