#ifndef QTR3DCAMERACONTROLLER_H
#define QTR3DCAMERACONTROLLER_H

#include <QVector3D>

class Qtr3dGeometryState;

class Qtr3dCameraController
{
public:
    enum Type {
        FreeFlyCamera,
        OrbitCamera,
        FollowCamera,
        FirstPersonCamera,
        ThirdPersonCamera,
        ChaseCamera,
        TopDownCamera
    };

    explicit Qtr3dCameraController(Type type);

    Type type() const;
    Qtr3dGeometryState *targetState() const;
    QVector3D targetPos() const;
    bool hasTargetPos() const;
    float distance() const;
    float offset() const;
    float height() const;
    QVector3D lookAtPos() const;
    bool hasLookAtPos() const;

    void setTargetState(Qtr3dGeometryState *targetState);
    void setTargetPos(const QVector3D &targetPos);
    void setDistance(float distance);
    void setOffset(float offset);
    void setHeight(float height);
    void setLookAtPos(const QVector3D &lookAtPos);

private:
    Type mType;
    Qtr3dGeometryState *mTargetState;
    QVector3D mTargetPos;
    bool mHasTargetPos;
    float mDistance;
    float mOffset;
    float mHeight;
    QVector3D mLookAtPos;
    bool mHasLookAtPos;
};

namespace Qtr3d {

Qtr3dCameraController freeFlyCamera();
Qtr3dCameraController firstPersonCamera(const QVector3D &startPos = QVector3D(0,0,10), const QVector3D &lookAt = QVector3D(0,0,0));
Qtr3dCameraController orbitCamera(const QVector3D &center = QVector3D(0,0,0), float distance = -1);
Qtr3dCameraController orbitCamera(Qtr3dGeometryState *targetState, float distance = -1);
Qtr3dCameraController followCamera(Qtr3dGeometryState *targetState, float distance = -1, float offset = 0);
Qtr3dCameraController thirdPersonCamera(Qtr3dGeometryState *targetState, float distance = 10, float height = 3);
Qtr3dCameraController chaseCamera(Qtr3dGeometryState *targetState, float distance = 10, float height = 3);
Qtr3dCameraController topDownCamera(const QVector3D &center = QVector3D(0,0,0), float height = 100);
Qtr3dCameraController topDownCamera(Qtr3dGeometryState *targetState, float height = 100);

}

#endif // QTR3DCAMERACONTROLLER_H
