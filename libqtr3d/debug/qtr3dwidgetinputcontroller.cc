#include <QKeyEvent>
#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>
#include "qtr3dwidgetinputcontroller.h"
#include "../qtr3dwidget.h"

//---------------------------------------------------------------------------------------
Qtr3dWidgetInputController::Qtr3dWidgetInputController(Qtr3dWidget *parent)
    : QObject(parent)
    , mIsDragging(false)
{
    Q_ASSERT(parent);
    parent->installEventFilter(this);
    parent->setMouseTracking(true);
}

//---------------------------------------------------------------------------------------
Qtr3dWidgetInputController::~Qtr3dWidgetInputController() = default;

//---------------------------------------------------------------------------------------
bool Qtr3dWidgetInputController::eventFilter(QObject *o, QEvent *event)
{
    switch(event->type()) {
    case QEvent::KeyPress: {
        QKeyEvent *ke = dynamic_cast<QKeyEvent*>(event); Q_ASSERT(ke);
        parentKeyPress(ke);
    } break;
    case QEvent::KeyRelease: {
        QKeyEvent *ke = dynamic_cast<QKeyEvent*>(event); Q_ASSERT(ke);
        parentKeyRelease(ke);
    } break;
    case QEvent::MouseMove:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseButtonDblClick:
    {
        QMouseEvent *me = dynamic_cast<QMouseEvent*>(event); Q_ASSERT(me);
        parentMouseEvent(me);
    } break;
    case QEvent::Wheel: {
        QWheelEvent *we = dynamic_cast<QWheelEvent*>(event); Q_ASSERT(we);
        parentMouseWheel(we->angleDelta().y());
    } break;
    default: break;
    }
    return QObject::eventFilter(o,event);
}

//---------------------------------------------------------------------------------------
void Qtr3dWidgetInputController::parentKeyPress(QKeyEvent*)
{
}

//---------------------------------------------------------------------------------------
void Qtr3dWidgetInputController::parentKeyRelease(QKeyEvent*)
{
}

//---------------------------------------------------------------------------------------
void Qtr3dWidgetInputController::parentMouseEvent(QMouseEvent *e)
{
    if (e->type() == QEvent::MouseButtonPress && e->button() == Qt::LeftButton) {
        parentDragStart(e->pos());
    }
    if (e->type() == QEvent::MouseButtonRelease && e->button() == Qt::LeftButton) {
        parentDragEnd();
    }
    if (e->type() == QEvent::MouseMove && mIsDragging) {
        QPoint absDelta = e->pos() - mLastDragPos;
        parentDraggingAbs(absDelta);
        parentDraggingRel(QPointF(absDelta.x()/(float)parentWidget()->width(),
                                  absDelta.y()/(float)parentWidget()->height()));

        mLastDragPos = e->pos();
    }
}

//---------------------------------------------------------------------------------------
void Qtr3dWidgetInputController::parentMouseWheel(float /*delta*/)
{

}

//---------------------------------------------------------------------------------------
void Qtr3dWidgetInputController::parentDragStart(QPoint startPos)
{
    mLastDragPos  = startPos;
    mIsDragging   = true;
}

//---------------------------------------------------------------------------------------
void Qtr3dWidgetInputController::parentDraggingAbs(QPoint /*delta*/)
{
}

//---------------------------------------------------------------------------------------
void Qtr3dWidgetInputController::parentDraggingRel(QPointF /*delta*/)
{
}

//---------------------------------------------------------------------------------------
void Qtr3dWidgetInputController::parentDragEnd()
{
    mIsDragging   = false;
    mLastDragPos  = QPoint();
}

//---------------------------------------------------------------------------------------
QWidget *Qtr3dWidgetInputController::parentWidget()
{
    QWidget *parentw = dynamic_cast<QWidget*>(parent());
    Q_ASSERT(parentw);
    return parentw;
}
