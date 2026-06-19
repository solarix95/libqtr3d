#ifndef QTR3DFIRSTPERSONCAMERACONTROLLER_H
#define QTR3DFIRSTPERSONCAMERACONTROLLER_H

#include <QVector3D>
#include "qtr3dwidgetinputcontroller.h"

class Qtr3dWidget;
class Qtr3dCamera;

class Qtr3dFirstPersonCameraController : public Qtr3dWidgetInputController
{
    Q_OBJECT
public:
    explicit Qtr3dFirstPersonCameraController(Qtr3dWidget *parent, float moveStep = 0.5f);

protected:
    virtual void parentKeyPress(QKeyEvent *e) override;
    virtual void parentMouseWheel(float delta) override;
    virtual void parentDraggingRel(QPointF delta) override;

private:
    QVector3D forward() const;
    QVector3D right() const;
    void moveCamera(const QVector3D &delta);

    Qtr3dCamera *mCamera;
    float mMoveStep;
};

#endif // QTR3DFIRSTPERSONCAMERACONTROLLER_H
