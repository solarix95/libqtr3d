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
    const Qtr3d::Material &cMaterial() const;
    void            setBlendingEnabled(bool enabled);
    bool            blending() const;

    virtual QVector3D minValues() const; // lowest xyz
    virtual QVector3D maxValues() const; // max xyz
    virtual QVector3D center() const;
    virtual double    radius() const;

    // BufferState Iteration
    virtual void registerBufferState(Qtr3dGeometryBufferState *s);
    inline const Qtr3dGeometryBufferStates &bufferStates() const { return mBufferStates; }
    inline QColor    defaultColor()   const { return mDefaultColor; }
    inline QVector3D defaultColorF()  const { return QVector3D(mDefaultColor.redF(), mDefaultColor.greenF(), mDefaultColor.blueF()); }
    inline QVector4D defaultColorF4() const { return QVector4D(mDefaultColor.redF(), mDefaultColor.greenF(), mDefaultColor.blueF(), mDefaultColor.alphaF()); }

    // Instances, "Blades-on-a-Turbine"-Usecase. One Mesh, multiple transitions:
    void addModelTransition(const QMatrix4x4 &t);
    void addModelTransition(const QVector3D &pos, const QVector3D &rotation = QVector3D());
    inline const QList<QMatrix4x4> &modelTransitions() const { return  mTransitions; }

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
    QColor                      mDefaultColor;

    QVector3D                   mMin;
    QVector3D                   mMax;

    QList<QMatrix4x4>           mTransitions;
};

typedef QList<Qtr3dGeometryBuffer> Qtr3dGeometryBuffers;

#endif // QTR3DGEOMETRYBUFFER_H
