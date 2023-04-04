#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QElapsedTimer>
#include <libqtr3d/qtr3dwidget.h>

class MainView : public Qtr3dWidget
{
    Q_OBJECT
public:
    MainView(QWidget *parent = nullptr);
    virtual ~MainView();

    inline int stateCount() const { return mStateCount; }
    inline float cpuLoad() const  { return mCpuLoad;    }

signals:
    void statisticsChanged();

protected:
    virtual void paint3D();
    virtual void paint2D();

private:
    QElapsedTimer mFpsTime;
    int           mStateCount;
    float         mCpuLoad;
    QList<int>    mElapsedTimes;
};

#endif // MAINVIEW_H
