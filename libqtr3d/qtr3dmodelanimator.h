#ifndef QTR3DMODELANIMATOR_H
#define QTR3DMODELANIMATOR_H

#include <QObject>
#include <QElapsedTimer>
#include <QStringList>
#include <QMatrix4x4>
#include <QMap>
#include <QVector3D>
#include "qtr3dmodelanimation.h"

class Qtr3dModelAnimator : public QObject
{
public:
    Qtr3dModelAnimator(Qtr3dModelAnimation *animation);

    void       start();
    void       stop();
    void       setState(bool animationRunning);
    void       setEnabled(bool enabled);

    void       setLoop(bool loop);
    // Freeze the source animator's current pose and blend it into this
    // animator. The duration is expressed in this animation's ticks.
    void       transitionFrom(const Qtr3dModelAnimator *sourceAnimator, float durationTicks);
    bool       transitionActive() const;
    bool       transform(const QString &nodeName, QMatrix4x4 &nodeTransform) const;
    void       setTicksPerSecond(int tps);
    float      animationTime() const;

private:
    float logicalAnimationTime() const;

    Qtr3dModelAnimation *mAnimation;
    QElapsedTimer        mAnimationTimer;
    int                  mOverrideTickesPerSecond;
    bool                 mIsEnabled;
    bool                 mLoop;
    QMap<QString,Qtr3dModelAnimation::NodeTransform> mTransitionSourcePose;
    float                mTransitionDurationTicks;
};

#endif // QTR3DMODELANIMATOR_H
