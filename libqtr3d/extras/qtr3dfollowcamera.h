#ifndef QTR3DFOLLOWCAMERA_H
#define QTR3DFOLLOWCAMERA_H

#include <QObject>

class Qtr3dWidget;
class Qtr3dCamera;
class Qtr3dGeometryState;

class Qtr3dFollowCamera : public QObject
{
    Q_OBJECT
public:
    Qtr3dFollowCamera(Qtr3dWidget *parent, Qtr3dGeometryState *followState, float distance = -1);
    virtual ~Qtr3dFollowCamera();

public slots:
    void process();

private slots:
    void follow();

private:
    Qtr3dCamera        *mCamera;
    Qtr3dGeometryState *mState;
    float               mDistance;
};

#endif // QTR3DFOLLOWCAMERA_H
