#ifndef QTR3DGEOMETRY_H
#define QTR3DGEOMETRY_H

#include <QObject>
#include <QList>
#include <QVector3D>
#include <QVector4D>
#include "qtr3dgeometrystate.h"
#include "qtr3dassets.h"
#include "qtr3dtypes.h"

class Qtr3dGeometryState;

//-------------------------------------------------------------------------------------------------
class Qtr3dGeometry : public QObject
{
    Q_OBJECT
public:
    Qtr3dGeometry(Qtr3dAssets *parent);
    virtual ~Qtr3dGeometry();

    Qtr3dAssets        *context();
    Qtr3d::ShaderType   shader() const;
    void                setFaceOrientation(Qtr3d::FaceOrientation orientation);
    Qtr3d::FaceOrientation faceOrientation() const;

    void            setDefaultColor(const QColor &c);
    Qtr3d::Material &material();
    const Qtr3d::Material &material() const;
    void            setBlendingEnabled(bool enabled);
    bool            blending() const;

    virtual QVector3D minValues() const; // lowest xyz
    virtual QVector3D maxValues() const; // max xyz
    virtual QVector3D center() const;
    virtual double    radius() const;

    // BufferState Iteration
    virtual void registerBufferState(Qtr3dGeometryState *s);
    inline const Qtr3dGeometryBufferStates &bufferStates() const { return mBufferStates; }

    void                 setParentBuffer(Qtr3dGeometry *parentBuffer);
    Qtr3dGeometry *parentBuffer() const;

protected:
    void analyze(const QVector3D &v);
    void setShader(Qtr3d::ShaderType t);

private slots:
    void stateDestroyed(QObject *state);

private:
    Qtr3dAssets               *mContext;
    Qtr3dGeometry        *mParentBuffer;

    Qtr3d::ShaderType           mShader;
    Qtr3d::FaceOrientation      mFaceOrientation;
    bool                        mBlending;
    Qtr3dGeometryBufferStates   mBufferStates;

    Qtr3d::Material             mMaterial;

    QVector3D                   mMin;
    QVector3D                   mMax;
};

typedef QList<Qtr3dGeometry> Qtr3dGeometryBuffers;

#endif // QTR3DGEOMETRY_H
