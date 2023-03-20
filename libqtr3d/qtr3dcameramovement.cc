#include <QTimer>
#include "qtr3dcameramovement.h"

//-------------------------------------------------------------------------------------------------
Qtr3dCameraMovement::Qtr3dCameraMovement(Qtr3dCamera *cam, int fps)
{
    Q_ASSERT(cam);
    Q_ASSERT(fps > 0);

    mCamera = cam;
    mFps    = fps;

    QTimer *t = new QTimer(this);
    connect(t, &QTimer::timeout, this, &Qtr3dCameraMovement::update);
    t->start(1000/mFps);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dCameraMovement::update()
{
    process();
    emit updated();
}
