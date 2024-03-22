#ifndef QTR3DGEOMETRYSTATE_H
#define QTR3DGEOMETRYSTATE_H

#include <QObject>
#include <QMatrix4x4>
#include <QList>
#include "math3d/qtr3ddblmatrix.h"
#include "math3d/qtr3ddblvector.h"
#include "qtr3dtypes.h"

class Qtr3dGeometry;
class Qtr3dModelAnimator;

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

    inline bool                    enabled() const { return mEnabled; }
    inline const Qtr3dDblVector3D &pos() const     { return mPos; }
    inline const QVector3D        &rot() const     { return mRot; }

    void setPos(const QVector3D &pos);
    void move(const QVector3D &pos);
    void move(const QVector3D &pos, const QVector3D &rotation);
    void setState(const QVector3D &pos, const QVector3D &rotation, const QVector3D &scale = {1,1,1});
    void setScale(const QVector3D &scale);
    void setScale(float xyzScale);
    void setRotation(const QVector3D &rotationAngles);
    void setModelView(const Qtr3dDblMatrix4x4 &modelView); // override pos, rotation, scale, ... calculate everything from outside.
    float radius() const;            // scaled buffer radius
    Qtr3dDblVector3D  center() const;        // absolute center

    inline const Qtr3dDblMatrix4x4  &modelView() const  { return mModelView; };
    inline const QVector3D          &scale()     const  { return mScale;     };
    inline const Qtr3dGeometry      &buffer()    const  { return mBuffer;    };

    void                       setAnimator(Qtr3dModelAnimator *animator);
    inline Qtr3dModelAnimator *animator() { return mAnimator; }

signals:
    void updated();

private:
    void updateMatrix();

    Qtr3dGeometry    &mBuffer;
    bool              mEnabled;
    Qtr3dDblMatrix4x4 mModelView;
    Qtr3dDblVector3D  mPos;
    QVector3D         mRot;
    QVector3D         mScale;

    Qtr3d::LightingType mLightingType;

    Qtr3dModelAnimator *mAnimator;
};

//-------------------------------------------------------------------------------------------------
typedef QList<Qtr3dGeometryState*> Qtr3dGeometryBufferStates;
#endif // QTR3DGEOMETRYSTATE_H
