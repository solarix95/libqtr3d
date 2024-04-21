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

    // Positioning
    inline const Qtr3dDblVector3D &pos() const     { return mPos; }
    void setPos(const Qtr3dDblVector3D &pos);
    void move(const Qtr3dDblVector3D &delta);

    // ModelView
    void setState(const Qtr3dDblVector3D &pos, const QVector3D &rotation, const QVector3D &scale = {1,1,1});
    void setScale(const QVector3D &scale);
    void setScale(float xyzScale);
    void setRotation(const QVector3D &rotationAngles);
    inline const QVector3D        &rot() const     { return mRot; }
    void setModelView(const QMatrix4x4 &modelView); // override pos, rotation, scale, ... calculate everything from outside.

    float radius() const;            // scaled buffer radius
    Qtr3dDblVector3D  center() const;        // absolute center

    const QMatrix4x4                 modelView(const Qtr3dDblVector3D &cameraPos = Qtr3dDblVector3D{0,0,0})  const;
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

    enum ModelViewFlag {
        NoFlag     = 0x00,
        PosFlag    = 0x01,
        ScaleFlag  = 0x02,
        RotateFlag = 0x04
    };
    Q_DECLARE_FLAGS(ModelViewFlags, ModelViewFlag);

    enum ModelViewMode {
        DefaultModelViewMode, // ModelView calculated by Scale + Rotation
        CustomMatrixViewMode  // ModelView created/set by external controller
    };

    ModelViewFlags    mModelViewFlags;
    ModelViewMode     mModelViewMode;
    Qtr3dDblVector3D  mPos;


    // DefaultModelViewMode
    QVector3D         mRot;
    QVector3D         mScale;

    // CustomMatrixViewMode
    QMatrix4x4        mModelView;

    Qtr3d::LightingType mLightingType;

    Qtr3dModelAnimator *mAnimator;
};

//-------------------------------------------------------------------------------------------------
typedef QList<Qtr3dGeometryState*> Qtr3dGeometryBufferStates;
#endif // QTR3DGEOMETRYSTATE_H
