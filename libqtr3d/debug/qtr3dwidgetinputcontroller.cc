#include <QKeyEvent>
#include <QMouseEvent>
#include "qtr3dwidgetinputcontroller.h"
#include "../qtr3dwidget.h"

//---------------------------------------------------------------------------------------
Qtr3dWidgetInputController::Qtr3dWidgetInputController(Qtr3dWidget *parent)
    : QObject(parent)
{
    Q_ASSERT(parent);
    parent->installEventFilter(this);
}

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
void Qtr3dWidgetInputController::parentMouseEvent(QMouseEvent*)
{
}
