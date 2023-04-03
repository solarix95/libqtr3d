#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QTime>
#include <libqtr3d/qtr3dwidget.h>


class MainView : public Qtr3dWidget
{
public:
    MainView();

protected:
    virtual void paint3D();
    virtual void paint2D();

private:
    QTime      mFpsTime;
    int        mStateCount;
    QList<int> mElapsedTimes;
};

#endif // MAINVIEW_H
