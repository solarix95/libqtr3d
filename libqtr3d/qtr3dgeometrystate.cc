#include "qtr3dgeometry.h"
#include "qtr3dgeometrystate.h"
#include "qtr3dmodelanimator.h"

//------------------------------------------------------------------------------------------------
Qtr3dGeometryState::Qtr3dGeometryState(Qtr3dGeometry *parent, Qtr3d::LightingType ltype)
    : QObject(parent)
    , mBuffer(*parent)
    , mEnabled(true)
    , mModelViewFlags(NoFlag)
    , mModelViewMode(DefaultModelViewMode)
    , mOrientation(1.0f, 0.0f, 0.0f, 0.0f)
    , mModelOrientation(1.0f, 0.0f, 0.0f, 0.0f)
    , mMotionDirection(0.0f, 0.0f, 0.0f)
    , mHasMotionDirection(false)
    , mLightingType(ltype)
    , mAnimator(nullptr)
{
    setState({0,0,0},{0,0,0},{1,1,1});
}

//------------------------------------------------------------------------------------------------
Qtr3dGeometryState::Qtr3dGeometryState(Qtr3dGeometry *parent, const QVector3D &pos, const QVector3D &rotation)
    : QObject(parent)
    , mBuffer(*parent)
    , mEnabled(true)
    , mModelViewFlags(NoFlag)
    , mModelViewMode(DefaultModelViewMode)
    , mOrientation(1.0f, 0.0f, 0.0f, 0.0f)
    , mModelOrientation(1.0f, 0.0f, 0.0f, 0.0f)
    , mMotionDirection(0.0f, 0.0f, 0.0f)
    , mHasMotionDirection(false)
    , mLightingType(Qtr3d::DefaultLighting)
    , mAnimator(nullptr)
{
    setState(pos,rotation,{1,1,1});
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::setLightingType(Qtr3d::LightingType ltype)
{
    mLightingType = ltype;
}

//------------------------------------------------------------------------------------------------
Qtr3d::LightingType Qtr3dGeometryState::lightingType() const
{
    return mLightingType;
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::setEnabled(bool enabled)
{
    mEnabled = enabled;
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::setPos(const Qtr3dDblVector3D &pos)
{
    mPos = pos;
    updateMatrix();
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::move(const Qtr3dDblVector3D &delta)
{
    mPos = mPos + delta;
    updateMatrix();
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::setState(const Qtr3dDblVector3D &pos, const QVector3D &rotation, const QVector3D &scale)
{
    mModelViewMode = DefaultModelViewMode;
    mPos   = pos;
    mRot   = rotation;
    mScale = scale;
    updateOrientationFromRotation();
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::setScale(const QVector3D &scale)
{
    mScale = scale;
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::setScale(float xyzScale)
{
    setScale({xyzScale,xyzScale,xyzScale});
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::setRotation(const QVector3D &rotationAngles)
{
    mModelViewMode = DefaultModelViewMode;
    mRot = rotationAngles;
    updateOrientationFromRotation();
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::updateOrientationFromRotation()
{
    QQuaternion orientation(1.0f, 0.0f, 0.0f, 0.0f);
    orientation *= QQuaternion::fromAxisAndAngle({1.0f, 0.0f, 0.0f}, mRot.x());
    orientation *= QQuaternion::fromAxisAndAngle({0.0f, 1.0f, 0.0f}, mRot.y());
    orientation *= QQuaternion::fromAxisAndAngle({0.0f, 0.0f, 1.0f}, mRot.z());
    mOrientation = orientation.normalized();
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::setOrientation(const QQuaternion &orientation)
{
    mModelViewMode = OrientationModelViewMode;
    mOrientation = orientation.normalized();
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::setModelOrientation(const QQuaternion &orientation)
{
    mModelOrientation = orientation.normalized();
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::setModelRotation(const QVector3D &rotationAngles)
{
    QQuaternion orientation(1.0f, 0.0f, 0.0f, 0.0f);
    orientation *= QQuaternion::fromAxisAndAngle({1.0f, 0.0f, 0.0f}, rotationAngles.x());
    orientation *= QQuaternion::fromAxisAndAngle({0.0f, 1.0f, 0.0f}, rotationAngles.y());
    orientation *= QQuaternion::fromAxisAndAngle({0.0f, 0.0f, 1.0f}, rotationAngles.z());
    setModelOrientation(orientation);
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::rotateLocal(const QVector3D &axis, float angleDegrees)
{
    if (qFuzzyIsNull(angleDegrees))
        return;

    setOrientation((mOrientation * QQuaternion::fromAxisAndAngle(axis, angleDegrees)).normalized());
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::setPitch(float angleDegrees)
{
    rotateLocal({1.0f, 0.0f, 0.0f}, angleDegrees);
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::setYaw(float angleDegrees)
{
    rotateLocal({0.0f, 1.0f, 0.0f}, angleDegrees);
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::setRoll(float angleDegrees)
{
    rotateLocal({0.0f, 0.0f, 1.0f}, angleDegrees);
}

//------------------------------------------------------------------------------------------------
QVector3D Qtr3dGeometryState::forward() const
{
    return mOrientation.rotatedVector({0.0f, 0.0f, -1.0f});
}

//------------------------------------------------------------------------------------------------
QVector3D Qtr3dGeometryState::right() const
{
    return mOrientation.rotatedVector({1.0f, 0.0f, 0.0f});
}

//------------------------------------------------------------------------------------------------
QVector3D Qtr3dGeometryState::up() const
{
    return mOrientation.rotatedVector({0.0f, 1.0f, 0.0f});
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::setMotionDirection(const QVector3D &direction)
{
    if (direction.isNull()) {
        clearMotionDirection();
        return;
    }

    mMotionDirection = direction.normalized();
    mHasMotionDirection = true;
}

//------------------------------------------------------------------------------------------------
QVector3D Qtr3dGeometryState::motionDirection() const
{
    return mMotionDirection;
}

//------------------------------------------------------------------------------------------------
bool Qtr3dGeometryState::hasMotionDirection() const
{
    return mHasMotionDirection;
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::clearMotionDirection()
{
    mMotionDirection = {0.0f, 0.0f, 0.0f};
    mHasMotionDirection = false;
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::setModelView(const QMatrix4x4 &modelView)
{
    mModelViewMode = CustomMatrixViewMode;
    mModelView = modelView;

    /*
    if (mModelView == modelView)
        return;

    mModelView = modelView;
    mPos       = mModelView * Qtr3dDblVector3D(0,0,0);
    emit updated();
    */
}

//------------------------------------------------------------------------------------------------
float Qtr3dGeometryState::radius() const
{
    return qMax(mScale.x(), qMax(mScale.y(), mScale.z())) * mBuffer.radius();
}

//------------------------------------------------------------------------------------------------
Qtr3dDblVector3D Qtr3dGeometryState::center() const
{
    return mPos + Qtr3dDblVector3D(mScale * mBuffer.center());
}

//------------------------------------------------------------------------------------------------
const QMatrix4x4 Qtr3dGeometryState::modelView(const Qtr3dDblVector3D &cameraPos) const
{
    QMatrix4x4 m;
    m.setToIdentity();

    m.translate((mPos-cameraPos).toFloat());

    switch (mModelViewMode) {
    case DefaultModelViewMode: {
        if (!mRot.isNull()) {
            m.rotate(mRot.x(),1,0,0);
            m.rotate(mRot.y(),0,1,0);
            m.rotate(mRot.z(),0,0,1);
        }
        m.rotate(mModelOrientation);
        if (!mScale.isNull())
            m.scale(mScale);
    } break;
    case OrientationModelViewMode: {
        m.rotate(mOrientation);
        m.rotate(mModelOrientation);
        if (!mScale.isNull())
            m.scale(mScale);
    } break;
    case CustomMatrixViewMode: {
        m *= mModelView;
    } break;
    }

    return m;
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::setAnimator(Qtr3dModelAnimator *animator)
{
    if (mAnimator)
        mAnimator->deleteLater();
    mAnimator = animator;
}

//------------------------------------------------------------------------------------------------
void Qtr3dGeometryState::updateMatrix()
{
    /*
    mModelView.setToIdentity();

    mModelView.translate(mPos);

    mModelView.rotate(mRot.x(),1,0,0);
    mModelView.rotate(mRot.y(),0,1,0);
    mModelView.rotate(mRot.z(),0,0,1);

    mModelView.scale(mScale);

    QMatrix4x4 m;
    m.setToIdentity();

    m.translate(mPos);
    m.rotate(mRot.x(),1,0,0);
    m.rotate(mRot.y(),0,1,0);
    m.rotate(mRot.z(),0,0,1);

    m.scale(mScale);
    mModelView = m;


    */
    emit updated();
}
