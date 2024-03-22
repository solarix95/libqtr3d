#ifndef QTR3DFOLLOWCAMERA_H
#define QTR3DFOLLOWCAMERA_H

#include <QObject>
#include <QVector3D>
#include "../math3d/qtr3ddblvector.h"
#include "../utils/qtr3dpidcontroller.h"

class Qtr3dWidget;
class Qtr3dCamera;
class Qtr3dGeometryState;

class Qtr3dFollowCamera : public QObject
{
    Q_OBJECT
public:
    Qtr3dFollowCamera(Qtr3dWidget *parent, Qtr3dGeometryState *followState, float distance = -1, float offset = 0);
    virtual ~Qtr3dFollowCamera();

public slots:
    void process();
    void setTargetDirection(const QVector3D &direction);
    void setTargetOrientation(const QVector3D &orientation);
    void setOffset(float offset);

private slots:
    void follow();

private:
    Qtr3dCamera        *mCamera;
    Qtr3dGeometryState *mState;
    float               mDistance;
    QVector3D           mTargetDirection;
    QVector3D           mTargetOrientation;
    float               mOffset;

    Qtr3dPidController<Qtr3dDblVector3D> mPosController;
    Qtr3dPidController<Qtr3dDblVector3D> mDirController;
};

#endif // QTR3DFOLLOWCAMERA_H
