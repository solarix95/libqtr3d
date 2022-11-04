#ifndef QTR3DGEOMETRYBUFFERSTATE_H
#define QTR3DGEOMETRYBUFFERSTATE_H

#include <QObject>
#include <QMatrix4x4>
#include <QList>
#include "qtr3dtypes.h"

class Qtr3dGeometryBuffer;

//-------------------------------------------------------------------------------------------------
class Qtr3dGeometryBufferState : public QObject
{
public:

    Qtr3dGeometryBufferState(Qtr3d::LightingType ltype = Qtr3d::DefaultLighting);
    Qtr3dGeometryBufferState(const QVector3D &pos, const QVector3D &rotation);

    void setParent(Qtr3dGeometryBufferState *state);
    void setLightingType(Qtr3d::LightingType ltype);
    Qtr3d::LightingType lightingType() const;

    inline const QVector3D &pos() const { return mPos; }
    inline const QVector3D &rot() const { return mRot; }

    void move(const QVector3D &pos);
    void move(const QVector3D &pos, const QVector3D &rotation);
    void setState(const QVector3D &pos, const QVector3D &rotation, const QVector3D &scale = {1,1,1});

    inline const QMatrix4x4 modelView() const { return mParent ? (mParent->modelView() * mModelView) : mModelView; }

private:
    void updateMatrix();

    QMatrix4x4  mModelView;
    QVector3D   mPos;
    QVector3D   mRot;
    QVector3D   mScale;

    Qtr3d::LightingType mLightingType;

    Qtr3dGeometryBufferState *mParent;
};

//-------------------------------------------------------------------------------------------------
typedef QList<Qtr3dGeometryBufferState*> Qtr3dGeometryBufferStates;
#endif // QTR3DGEOMETRYBUFFERSTATE_H
