#ifndef QTR3DTHIRDPERSONCAMERACONTROLLER_H
#define QTR3DTHIRDPERSONCAMERACONTROLLER_H

#include <QObject>
#include <QVector3D>
#include "../math3d/qtr3ddblvector.h"

class Qtr3dWidget;
class Qtr3dCamera;
class Qtr3dGeometryState;

class Qtr3dThirdPersonCameraController : public QObject
{
    Q_OBJECT
public:
    Qtr3dThirdPersonCameraController(Qtr3dWidget *parent, Qtr3dGeometryState *targetState, float distance = 10.0f, float height = 3.0f);

public slots:
    void setTargetDirection(const QVector3D &direction);
    void updateCamera();

private:
    Qtr3dCamera *mCamera;
    Qtr3dGeometryState *mTargetState;
    QVector3D mTargetDirection;
    float mDistance;
    float mHeight;
};

#endif // QTR3DTHIRDPERSONCAMERACONTROLLER_H
