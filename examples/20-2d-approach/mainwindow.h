#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dgeometrystate.h>

class MainWindow : public Qtr3dWidget
{
public:
    MainWindow();

protected:
    void mouseMoveEvent(QMouseEvent *event);

private:
    Qtr3dGeometryState *mState;
};

#endif // MAINWINDOW_H
