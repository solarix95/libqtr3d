#ifndef QTR3DCHASECAMERACONTROLLER_H
#define QTR3DCHASECAMERACONTROLLER_H

#include "qtr3dthirdpersoncameracontroller.h"

class Qtr3dChaseCameraController : public Qtr3dThirdPersonCameraController
{
    Q_OBJECT
public:
    Qtr3dChaseCameraController(Qtr3dWidget *parent, Qtr3dGeometryState *targetState, float distance = 10.0f, float height = 3.0f);
};

#endif // QTR3DCHASECAMERACONTROLLER_H
