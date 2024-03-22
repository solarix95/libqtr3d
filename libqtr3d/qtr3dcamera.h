#ifndef QTR3DCAMERA_H
#define QTR3DCAMERA_H

#include <QObject>
#include <QMatrix4x4>
#include <QVector3D>
#include "math3d/qtr3ddblmatrix.h"

class Qtr3dWidget;
class Qtr3dCamera : public QObject
{
    Q_OBJECT

public:
    Qtr3dCamera(Qtr3dWidget *widget);

    Qtr3dWidget *widget();

    // Minimal Setup
    void setPos(float x, float y, float z);
    void setPos(double x, double y, double z);
    void setPos(const Qtr3dDblVector3D &pos);
    void move(float dx, float dy, float dz);
    void setFov(float angle);
    void setFov(float angle, float zNear, float zFar);
    void setGeometry(float width, float height);
    Qtr3dDblVector3D pos() const;
    Qtr3dDblVector3D lookAtCenter() const;

    // Camera Modes
    void lookAt(const Qtr3dDblVector3D &toCenter, const QVector3D &up);
    void lookAt(const Qtr3dDblVector3D &pos, const Qtr3dDblVector3D &toCenter, const QVector3D &up);

    void setupWorldMatrix(QMatrix4x4 &matrix, bool absolutPos = true);

    void lookAtTurn(float dxAngle, float dyAngle);
    void rotate(float xAngle, float yAngle, float zAngle);


    // Shader/Renderer-Interface
    inline QMatrix4x4 projection()  const  { return mProjectionMatrix.toFloat(); }
    inline QMatrix4x4 worldMatrix() const  { return mWorldMatrix.toFloat();      }

signals:
    void changed();
    void positionChanged(Qtr3dDblVector3D newPos);

private:
    void updatePerspectiveMatrix();

    Qtr3dWidget *mWidget;

    // Perspective
    float      mFov;
    float      mZNear;
    float      mZFar;
    float      mWidth;
    float      mHeight;

    // LookAt
    Qtr3dDblVector3D  mPos;
    Qtr3dDblVector3D  mLookAt;
    QVector3D         mUp;


    Qtr3dDblMatrix4x4 mWorldMatrix;
    Qtr3dDblMatrix4x4 mProjectionMatrix;
};

#endif // QTR3DCAMERA_H
