#include "qtr3dcameracontroller.h"

Qtr3dCameraController::Qtr3dCameraController(Type type)
    : mType(type)
    , mTargetState(nullptr)
    , mTargetPos(0,0,0)
    , mHasTargetPos(false)
    , mDistance(-1)
    , mOffset(0)
    , mHeight(0)
    , mLookAtPos(0,0,0)
    , mHasLookAtPos(false)
{
}

Qtr3dCameraController::Type Qtr3dCameraController::type() const
{
    return mType;
}

Qtr3dGeometryState *Qtr3dCameraController::targetState() const
{
    return mTargetState;
}

QVector3D Qtr3dCameraController::targetPos() const
{
    return mTargetPos;
}

bool Qtr3dCameraController::hasTargetPos() const
{
    return mHasTargetPos;
}

float Qtr3dCameraController::distance() const
{
    return mDistance;
}

float Qtr3dCameraController::offset() const
{
    return mOffset;
}

float Qtr3dCameraController::height() const
{
    return mHeight;
}

QVector3D Qtr3dCameraController::lookAtPos() const
{
    return mLookAtPos;
}

bool Qtr3dCameraController::hasLookAtPos() const
{
    return mHasLookAtPos;
}

void Qtr3dCameraController::setTargetState(Qtr3dGeometryState *targetState)
{
    mTargetState = targetState;
}

void Qtr3dCameraController::setTargetPos(const QVector3D &targetPos)
{
    mTargetPos = targetPos;
    mHasTargetPos = true;
}

void Qtr3dCameraController::setDistance(float distance)
{
    mDistance = distance;
}

void Qtr3dCameraController::setOffset(float offset)
{
    mOffset = offset;
}

void Qtr3dCameraController::setHeight(float height)
{
    mHeight = height;
}

void Qtr3dCameraController::setLookAtPos(const QVector3D &lookAtPos)
{
    mLookAtPos = lookAtPos;
    mHasLookAtPos = true;
}

Qtr3dCameraController Qtr3d::freeFlyCamera()
{
    return Qtr3dCameraController(Qtr3dCameraController::FreeFlyCamera);
}

Qtr3dCameraController Qtr3d::firstPersonCamera(const QVector3D &startPos, const QVector3D &lookAt)
{
    Qtr3dCameraController ret(Qtr3dCameraController::FirstPersonCamera);
    ret.setTargetPos(startPos);
    ret.setLookAtPos(lookAt);
    return ret;
}

Qtr3dCameraController Qtr3d::orbitCamera(const QVector3D &center, float distance)
{
    Qtr3dCameraController ret(Qtr3dCameraController::OrbitCamera);
    ret.setTargetPos(center);
    ret.setDistance(distance);
    return ret;
}

Qtr3dCameraController Qtr3d::orbitCamera(Qtr3dGeometryState *targetState, float distance)
{
    Qtr3dCameraController ret(Qtr3dCameraController::OrbitCamera);
    ret.setTargetState(targetState);
    ret.setDistance(distance);
    return ret;
}

Qtr3dCameraController Qtr3d::followCamera(Qtr3dGeometryState *targetState, float distance, float offset)
{
    Qtr3dCameraController ret(Qtr3dCameraController::FollowCamera);
    ret.setTargetState(targetState);
    ret.setDistance(distance);
    ret.setOffset(offset);
    return ret;
}

Qtr3dCameraController Qtr3d::thirdPersonCamera(Qtr3dGeometryState *targetState, float distance, float height)
{
    Qtr3dCameraController ret(Qtr3dCameraController::ThirdPersonCamera);
    ret.setTargetState(targetState);
    ret.setDistance(distance);
    ret.setHeight(height);
    return ret;
}

Qtr3dCameraController Qtr3d::chaseCamera(Qtr3dGeometryState *targetState, float distance, float height)
{
    Qtr3dCameraController ret(Qtr3dCameraController::ChaseCamera);
    ret.setTargetState(targetState);
    ret.setDistance(distance);
    ret.setHeight(height);
    return ret;
}

Qtr3dCameraController Qtr3d::topDownCamera(const QVector3D &center, float height)
{
    Qtr3dCameraController ret(Qtr3dCameraController::TopDownCamera);
    ret.setTargetPos(center);
    ret.setHeight(height);
    return ret;
}

Qtr3dCameraController Qtr3d::topDownCamera(Qtr3dGeometryState *targetState, float height)
{
    Qtr3dCameraController ret(Qtr3dCameraController::TopDownCamera);
    ret.setTargetState(targetState);
    ret.setHeight(height);
    return ret;
}
