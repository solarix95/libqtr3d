#ifndef QTR3DCHASECAMERACONTROLLER_H
#define QTR3DCHASECAMERACONTROLLER_H

#include <QObject>
#include <QElapsedTimer>
#include <QVector3D>
#include "../math3d/qtr3ddblvector.h"

class Qtr3dWidget;
class Qtr3dCamera;
class Qtr3dGeometryState;

class Qtr3dChaseCameraController : public QObject
{
    Q_OBJECT
public:
    enum OrientationMode {
        WorldUp,
        TargetUp
    };

    Qtr3dChaseCameraController(Qtr3dWidget *parent, Qtr3dGeometryState *targetState,
                               float distance = 10.0f, float height = 3.0f,
                               OrientationMode orientationMode = WorldUp,
                               float positionSmoothing = 8.0f, float lookAtSmoothing = 12.0f,
                               float snapDistance = 80.0f);

    void setPositionSmoothing(float smoothing);
    void setLookAtSmoothing(float smoothing);
    void setSnapDistance(float distance);

public slots:
    void setTargetDirection(const QVector3D &direction);
    void updateCamera();

private:
    Qtr3dCamera *mCamera;
    Qtr3dGeometryState *mTargetState;
    QVector3D mTargetDirection;
    float mDistance;
    float mHeight;
    OrientationMode mOrientationMode;
    float mPositionSmoothing;
    float mLookAtSmoothing;
    float mSnapDistance;
    bool mInitialized;
    QElapsedTimer mElapsed;
    Qtr3dDblVector3D mCurrentPos;
    Qtr3dDblVector3D mCurrentLookAt;
    QVector3D mCurrentUp;
};

#endif // QTR3DCHASECAMERACONTROLLER_H
