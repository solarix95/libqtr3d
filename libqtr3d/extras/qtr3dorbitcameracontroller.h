#ifndef QTR3DORBITCAMERACONTROLLER_H
#define QTR3DORBITCAMERACONTROLLER_H

#include <QEvent>
#include <QVector3D>
#include "qtr3dcameramovement.h"

class Qtr3dOrbitCameraController : public Qtr3dCameraMovement
{
    Q_OBJECT
public:
    Qtr3dOrbitCameraController(Qtr3dCamera *cam, int fps, float deltaAngleY,
                      const QVector3D &startPos,
                      const QVector3D &lookAt);
    Qtr3dOrbitCameraController(Qtr3dCamera *cam, int fps, const QVector3D &deltaAngle,
                      const QVector3D &startPos,
                      const QVector3D &lookAt);
    virtual ~Qtr3dOrbitCameraController();

protected:
    virtual void process();
    virtual bool eventFilter(QObject *obj, QEvent *event);

private:
    QVector3D mDeltaAngle;
    QVector3D mCurrentAngle;

    QVector3D mStartPos;
    QVector3D mTargetPos;
    QVector3D mDeltaPos;

    QVector3D mLookAt;
};

#endif // QTR3DORBITCAMERACONTROLLER_H
