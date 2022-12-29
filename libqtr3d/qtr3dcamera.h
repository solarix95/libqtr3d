#ifndef QTR3DCAMERA_H
#define QTR3DCAMERA_H

#include <QObject>
#include <QMatrix4x4>
#include <QVector3D>

class Qtr3dCamera : public QObject
{
    Q_OBJECT

public:
    Qtr3dCamera();

    // Minimal Setup
    void setPosition(float x, float y, float z);
    void setPosition(const QVector3D &pos);
    void move(float dx, float dy, float dz);
    void setFov(float angle);
    void setGeometry(float width, float height);
    QVector3D pos() const;
    QVector3D lookAtCenter() const;

    // Camera Modes
    void lookAt(const QVector3D &toCenter, const QVector3D &up);
    void lookAt(const QVector3D &pos, const QVector3D &toCenter, const QVector3D &up);
    void lookAtTurn(float dxAngle, float dyAngle);
    void rotate(float xAngle, float yAngle, float zAngle);

    // Shader/Renderer-Interface
    inline const QMatrix4x4 &projection() const  { return mProjectionMatrix; }
    inline const QMatrix4x4 &worldMatrix() const { return mWorldMatrix; }

signals:
    void changed();
    void positionChanged(QVector3D newPos);

private:
    void updatePerspectiveMatrix();

    enum Mode {
        Fixed,
        LookAt
    };

    QVector3D  mPos;
    float      mFov;    // Field of View
    float      mWidth;
    float      mHeight;

    // LookAt-Mode
    QVector3D  mLookAt;
    QVector3D  mUp;

    // Fixed Mode (default)
    QVector3D  mAngles;

    Mode       mMode;
    QMatrix4x4 mWorldMatrix;
    QMatrix4x4 mProjectionMatrix;
};

#endif // QTR3DCAMERA_H
