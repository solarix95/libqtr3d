#ifndef QTR3DFOLLOWCAMERA_H
#define QTR3DFOLLOWCAMERA_H

#include <QObject>

class Qtr3dWidget;
class Qtr3dCamera;
class Qtr3dGeometryBufferState;

class Qtr3dFollowCamera : public QObject
{
    Q_OBJECT
public:
    Qtr3dFollowCamera(Qtr3dWidget *parent, Qtr3dGeometryBufferState *followState);
    virtual ~Qtr3dFollowCamera();

public slots:
    void process();

private slots:
    void follow();

private:
    Qtr3dCamera              *mCamera;
    Qtr3dGeometryBufferState *mState;
};

#endif // QTR3DFOLLOWCAMERA_H
