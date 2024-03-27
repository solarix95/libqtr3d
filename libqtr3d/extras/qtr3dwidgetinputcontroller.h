#ifndef QTR3DWIDGETINPUTCONTROLLER_H
#define QTR3DWIDGETINPUTCONTROLLER_H

#include <QObject>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QPointF>

class Qtr3dWidget;
class Qtr3dWidgetInputController : public QObject
{
    Q_OBJECT
public:
    explicit Qtr3dWidgetInputController(QWidget *parent);
    virtual ~Qtr3dWidgetInputController();

protected:
    Qt::KeyboardModifiers modifiers() const;
    virtual bool eventFilter(QObject *watched, QEvent *event);

    virtual void parentKeyPress(QKeyEvent *e);
    virtual void parentKeyRelease(QKeyEvent *e);
    virtual void parentMouseEvent(QMouseEvent *e);

    virtual void parentMouseWheel(float delta);
    virtual void parentDragStart(QPoint startPos);
    virtual void parentDraggingAbs(QPoint delta);  // in Pixels
    virtual void parentDraggingRel(QPointF delta); // % of the screen
    virtual void parentDragEnd();

signals:
    void draggingRelative(QPointF delta);
    void mouseWheel(float delta);

private:
    QWidget *parentWidget();

    bool   mIsDragging;
    QPoint mLastDragPos;
    Qt::KeyboardModifiers mCurrentModifier;
};

#endif // QTR3DWIDGETINPUTCONTROLLER_H
