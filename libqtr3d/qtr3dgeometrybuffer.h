#ifndef QTR3DGEOMETRYBUFFER_H
#define QTR3DGEOMETRYBUFFER_H

#include <QObject>
#include <QList>
#include <QVector3D>
#include <QVector4D>
#include "qtr3dgeometrybufferstate.h"
#include "qtr3dcontext.h"
#include "qtr3dtypes.h"

class Qtr3dGeometryBufferState;

//-------------------------------------------------------------------------------------------------
class Qtr3dGeometryBuffer : public QObject
{
    Q_OBJECT
public:
    Qtr3dGeometryBuffer(Qtr3dContext *parent);
    virtual ~Qtr3dGeometryBuffer();

    Qtr3dContext        *context();
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
    virtual void registerBufferState(Qtr3dGeometryBufferState *s);
    inline const Qtr3dGeometryBufferStates &bufferStates() const { return mBufferStates; }

    void                 setParentBuffer(Qtr3dGeometryBuffer *parentBuffer);
    Qtr3dGeometryBuffer *parentBuffer() const;

protected:
    void analyze(const QVector3D &v);
    void setShader(Qtr3d::ShaderType t);

private slots:
    void stateDestroyed(QObject *state);

private:
    Qtr3dContext               *mContext;
    Qtr3dGeometryBuffer        *mParentBuffer;

    Qtr3d::ShaderType           mShader;
    Qtr3d::FaceOrientation      mFaceOrientation;
    bool                        mBlending;
    Qtr3dGeometryBufferStates   mBufferStates;

    Qtr3d::Material             mMaterial;

    QVector3D                   mMin;
    QVector3D                   mMax;
};

typedef QList<Qtr3dGeometryBuffer> Qtr3dGeometryBuffers;

#endif // QTR3DGEOMETRYBUFFER_H
