#include <QTimer>
#include <QtGlobal>
#include <cmath>
#include "qtr3dchasecameracontroller.h"
#include "../qtr3dcamera.h"
#include "../qtr3dgeometrystate.h"
#include "../qtr3dwidget.h"

namespace {

static float dampingFactor(float smoothing, double dtSeconds)
{
    if (smoothing <= 0.0f)
        return 1.0f;

    return 1.0f - std::exp(-smoothing * qMax(0.0, dtSeconds));
}

static Qtr3dDblVector3D dampedVector(const Qtr3dDblVector3D &current, const Qtr3dDblVector3D &target, float alpha)
{
    return current + (target-current) * static_cast<double>(alpha);
}

static QVector3D dampedVector(const QVector3D &current, const QVector3D &target, float alpha)
{
    return current + (target-current) * alpha;
}

}

Qtr3dChaseCameraController::Qtr3dChaseCameraController(Qtr3dWidget *parent, Qtr3dGeometryState *targetState,
                                                       float distance, float height,
                                                       OrientationMode orientationMode,
                                                       float positionSmoothing, float lookAtSmoothing,
                                                       float snapDistance)
    : QObject(parent)
    , mCamera(parent ? parent->camera() : nullptr)
    , mTargetState(targetState)
    , mTargetDirection()
    , mDistance(distance)
    , mHeight(height)
    , mOrientationMode(orientationMode)
    , mPositionSmoothing(positionSmoothing)
    , mLookAtSmoothing(lookAtSmoothing)
    , mSnapDistance(snapDistance)
    , mInitialized(false)
    , mCurrentPos()
    , mCurrentLookAt()
    , mCurrentUp(0, 1, 0)
{
    Q_ASSERT(parent);
    Q_ASSERT(mCamera);
    Q_ASSERT(mTargetState);

    mElapsed.start();

    auto *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Qtr3dChaseCameraController::updateCamera);
    timer->start(33);
    updateCamera();
}

void Qtr3dChaseCameraController::setPositionSmoothing(float smoothing)
{
    mPositionSmoothing = qMax(0.0f, smoothing);
}

void Qtr3dChaseCameraController::setLookAtSmoothing(float smoothing)
{
    mLookAtSmoothing = qMax(0.0f, smoothing);
}

void Qtr3dChaseCameraController::setSnapDistance(float distance)
{
    mSnapDistance = qMax(0.0f, distance);
}

void Qtr3dChaseCameraController::setTargetDirection(const QVector3D &direction)
{
    mTargetDirection = direction.isNull() ? QVector3D() : direction.normalized();
}

void Qtr3dChaseCameraController::updateCamera()
{
    const Qtr3dDblVector3D center = mTargetState->center();
    QVector3D forward = mTargetDirection;
    if (forward.isNull())
        forward = mTargetState->hasMotionDirection() ? mTargetState->motionDirection() : mTargetState->forward();
    const QVector3D up = mOrientationMode == TargetUp ? mTargetState->up() : QVector3D(0, 1, 0);

    const QVector3D cameraForward = forward.isNull() ? QVector3D(0, 0, -1) : forward.normalized();
    const QVector3D cameraUp = up.isNull() ? QVector3D(0, 1, 0) : up.normalized();

    const Qtr3dDblVector3D targetPos = center - Qtr3dDblVector3D(cameraForward*mDistance)
                                       + Qtr3dDblVector3D(cameraUp*mHeight);
    const Qtr3dDblVector3D targetLookAt = center + Qtr3dDblVector3D(cameraUp*(mHeight*0.35f));

    const double dtSeconds = qMin(0.25, mElapsed.restart() / 1000.0);
    const bool snap = !mInitialized
                      || (mSnapDistance > 0.0f && mCurrentPos.distanceToPoint(targetPos) > mSnapDistance);

    if (snap) {
        mCurrentPos = targetPos;
        mCurrentLookAt = targetLookAt;
        mCurrentUp = cameraUp;
        mInitialized = true;
    } else {
        const float posAlpha = dampingFactor(mPositionSmoothing, dtSeconds);
        const float lookAtAlpha = dampingFactor(mLookAtSmoothing, dtSeconds);
        mCurrentPos = dampedVector(mCurrentPos, targetPos, posAlpha);
        mCurrentLookAt = dampedVector(mCurrentLookAt, targetLookAt, lookAtAlpha);
        mCurrentUp = dampedVector(mCurrentUp, cameraUp, lookAtAlpha);
        if (mCurrentUp.isNull())
            mCurrentUp = cameraUp;
        else
            mCurrentUp.normalize();
    }

    mCamera->lookAt(mCurrentPos, mCurrentLookAt, mCurrentUp);
}
