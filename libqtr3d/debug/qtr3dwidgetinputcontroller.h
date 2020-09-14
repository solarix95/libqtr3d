#ifndef QTR3DWIDGETINPUTCONTROLLER_H
#define QTR3DWIDGETINPUTCONTROLLER_H

#include <QObject>
#include <QEvent>
#include <QKeyEvent>
#include <QMouseEvent>

class Qtr3dWidget;
class Qtr3dWidgetInputController : public QObject
{
    Q_OBJECT
public:
    explicit Qtr3dWidgetInputController(Qtr3dWidget *parent);

protected:
    virtual bool eventFilter(QObject *watched, QEvent *event);

    virtual void parentKeyPress(QKeyEvent *e);
    virtual void parentKeyRelease(QKeyEvent *e);
    virtual void parentMouseEvent(QMouseEvent *e);

};

#endif // QTR3DWIDGETINPUTCONTROLLER_H
