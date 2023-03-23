#ifndef QTR3DCAMERAMOVEMENT_H
#define QTR3DCAMERAMOVEMENT_H

#include <QObject>

class Qtr3dCamera;
class Qtr3dCameraMovement : public QObject
{
    Q_OBJECT
public:
    Qtr3dCameraMovement(Qtr3dCamera *cam, int fps);

    inline Qtr3dCamera *camera() const { return mCamera; }
    inline int          fps() const    { return mFps;    }

signals:
    void updated();

protected:
    virtual void process() = 0;

private slots:
    void update();

private:
    Qtr3dCamera *mCamera;
    int          mFps;
};

#endif // QTR3DCAMERAMOVEMENT_H
