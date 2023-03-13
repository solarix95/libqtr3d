#ifndef QTR3DEGOCAMERACONTROLLER_H
#define QTR3DEGOCAMERACONTROLLER_H

#include <QObject>
#include <QVector3D>
#include "qtr3dwidgetinputcontroller.h"
class Qtr3dWidget;
class Qtr3dCamera;

class Qtr3dEgoCameraController : public Qtr3dWidgetInputController
{
    Q_OBJECT
public:
    Qtr3dEgoCameraController(Qtr3dWidget *parent);
    ~Qtr3dEgoCameraController();

signals:
    void positionChanged(QVector3D pos);

protected:
    virtual void parentKeyPress(QKeyEvent *e) override;
    virtual void parentMouseWheel(float delta) override;
    virtual void parentDraggingRel(QPointF delta) override;

private:
    Qtr3dCamera *mCamera;
};

#endif
