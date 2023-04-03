#ifndef QTR3DGEOMETRYSTATE_H
#define QTR3DGEOMETRYSTATE_H

#include <QObject>
#include <QMatrix4x4>
#include <QList>
#include "qtr3dtypes.h"

class Qtr3dGeometry;

//-------------------------------------------------------------------------------------------------
class Qtr3dGeometryState : public QObject
{
    Q_OBJECT
public:
    Qtr3dGeometryState(Qtr3dGeometry *parent, Qtr3d::LightingType ltype = Qtr3d::DefaultLighting);
    Qtr3dGeometryState(Qtr3dGeometry *parent, const QVector3D &pos, const QVector3D &rotation);

    void setLightingType(Qtr3d::LightingType ltype);
    Qtr3d::LightingType lightingType() const;
    void setEnabled(bool enabled);

    inline bool             enabled() const { return mEnabled; }
    inline const QVector3D &pos() const     { return mPos; }
    inline const QVector3D &rot() const     { return mRot; }

    void setPos(const QVector3D &pos);
    void move(const QVector3D &pos);
    void move(const QVector3D &pos, const QVector3D &rotation);
    void setState(const QVector3D &pos, const QVector3D &rotation, const QVector3D &scale = {1,1,1});
    void setScale(const QVector3D &scale);
    void setScale(float xyzScale);
    void setRotation(const QVector3D &rotationAngles);
    void setModelView(const QMatrix4x4 &modelView); // override pos, rotation, scale, ... calculate everything from outside.
    float radius() const; // scaled buffer radius

    inline const QMatrix4x4 modelView()  const  { return mModelView; };
    inline const Qtr3dGeometry &buffer() const  { return mBuffer;    };

signals:
    void updated();

private:
    void updateMatrix();

    Qtr3dGeometry &mBuffer;
    bool        mEnabled;
    QMatrix4x4  mModelView;
    QVector3D   mPos;
    QVector3D   mRot;
    QVector3D   mScale;

    Qtr3d::LightingType mLightingType;
};

//-------------------------------------------------------------------------------------------------
typedef QList<Qtr3dGeometryState*> Qtr3dGeometryBufferStates;
#endif // QTR3DGEOMETRYSTATE_H
