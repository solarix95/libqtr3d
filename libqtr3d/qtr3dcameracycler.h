#ifndef QTR3DCAMERACYCLER_H
#define QTR3DCAMERACYCLER_H

#include <QVector3D>
#include "qtr3dcameramovement.h"

class Qtr3dCameraCycler : public Qtr3dCameraMovement
{
public:
    Qtr3dCameraCycler(Qtr3dCamera *cam, int fps, float deltaAngle,
                      const QVector3D &startPos,
                      const QVector3D &lookAt);

protected:
    virtual void process();

private:
    float mDeltaAngle;
    float mCurrentAngle;

    QVector3D mStartPos;
    QVector3D mLookAt;
};

#endif // QTR3DCAMERACYCLER_H
