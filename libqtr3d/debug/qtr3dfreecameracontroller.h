#ifndef QTR3DFREECAMERACONTROLLER_H
#define QTR3DFREECAMERACONTROLLER_H

#include <QObject>
#include <QVector3D>
#include "qtr3dwidgetinputcontroller.h"
class Qtr3dWidget;
class Qtr3dCamera;

class Qtr3dFreeCameraController : public Qtr3dWidgetInputController
{
    Q_OBJECT
public:
    Qtr3dFreeCameraController(Qtr3dWidget *parent);
    ~Qtr3dFreeCameraController();

signals:
    void positionChanged(QVector3D pos);

protected:
    virtual void parentKeyPress(QKeyEvent *e) override;
    virtual void parentMouseWheel(float delta) override;
    virtual void parentDraggingRel(QPointF delta) override;

private:
    Qtr3dCamera *mCamera;
};

#endif // QTR3DFREECAMERACONTROLLER_H
