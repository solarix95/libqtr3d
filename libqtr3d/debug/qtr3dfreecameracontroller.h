#ifndef QTR3DFREECAMERACONTROLLER_H
#define QTR3DFREECAMERACONTROLLER_H

#include <QObject>
#include "qtr3dwidgetinputcontroller.h"
class Qtr3dWidget;
class Qtr3dCamera;

class Qtr3dFreeCameraController : public Qtr3dWidgetInputController
{
public:
    Qtr3dFreeCameraController(Qtr3dWidget *parent);

protected:
    virtual void parentKeyPress(QKeyEvent *e);

private:
    Qtr3dCamera *mCamera;
};

#endif // QTR3DFREECAMERACONTROLLER_H
