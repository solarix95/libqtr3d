
#include <QEvent>
#include "qtr3dfreecameracontroller.h"
#include "../qtr3dwidget.h"
#include "../qtr3dcamera.h"

//-------------------------------------------------------------------------------------------------
Qtr3dFreeCameraController::Qtr3dFreeCameraController(Qtr3dWidget *parent)
 : Qtr3dWidgetInputController(parent)
{
    Q_ASSERT(parent);
    parent->installEventFilter(this);

    mCamera = parent->camera();
    Q_ASSERT(mCamera);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dFreeCameraController::parentKeyPress(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Up)
        mCamera->move(0,0.1,0);
    if (e->key() == Qt::Key_Down)
        mCamera->move(0,-0.1,0);
    if (e->key() == Qt::Key_Left)
        mCamera->move(-0.1,0,0);
    if (e->key() == Qt::Key_Right)
        mCamera->move(0.1,0,0);
}
