#include "qtr3dchasecameracontroller.h"

Qtr3dChaseCameraController::Qtr3dChaseCameraController(Qtr3dWidget *parent, Qtr3dGeometryState *targetState, float distance, float height)
    : Qtr3dThirdPersonCameraController(parent, targetState, distance, height)
{
}
