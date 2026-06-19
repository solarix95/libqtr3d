#ifndef QTR3DTOPDOWNCAMERACONTROLLER_H
#define QTR3DTOPDOWNCAMERACONTROLLER_H

#include <QVector3D>
#include "qtr3dwidgetinputcontroller.h"

class Qtr3dWidget;
class Qtr3dCamera;
class Qtr3dGeometryState;

class Qtr3dTopDownCameraController : public Qtr3dWidgetInputController
{
    Q_OBJECT
public:
    Qtr3dTopDownCameraController(Qtr3dWidget *parent, const QVector3D &center = QVector3D(0,0,0), float height = 100.0f);
    Qtr3dTopDownCameraController(Qtr3dWidget *parent, Qtr3dGeometryState *targetState, float height = 100.0f);

protected:
    virtual void parentKeyPress(QKeyEvent *e) override;
    virtual void parentMouseWheel(float delta) override;
    virtual void parentDraggingRel(QPointF delta) override;

private slots:
    void updateCamera();

private:
    Qtr3dCamera *mCamera;
    Qtr3dGeometryState *mTargetState;
    QVector3D mCenter;
    float mHeight;
    float mPanStep;
};

#endif // QTR3DTOPDOWNCAMERACONTROLLER_H
