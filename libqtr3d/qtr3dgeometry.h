#ifndef QTR3DGEOMETRY_H
#define QTR3DGEOMETRY_H

#include <QObject>
#include <QList>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>
#include <QFlags>
#include "qtr3dgeometrystate.h"
#include "qtr3dassets.h"
#include "qtr3dtypes.h"

class Qtr3dGeometryState;

//-------------------------------------------------------------------------------------------------
class Qtr3dGeometry : public QObject
{
    Q_OBJECT
public:
    enum Pipeline {
        MeshPipeline,
        ModelPipeline,
        PointCloudPipeline
    };

    enum RenderOption {
        DefaultRenderOption = 0x00,
        ZOrderOption        = 0x01,
        BlendingOption      = 0x02,
        NoZBufferOption     = 0x04,
        BackgroundOption    = 0x08,  // Skybox. Rendered First

        BlendingOptions     = BlendingOption | ZOrderOption,
    };
    Q_DECLARE_FLAGS(RenderOptions, RenderOption)

    Qtr3dGeometry(Pipeline t, Qtr3dAssets *parent);
    virtual ~Qtr3dGeometry();

    Pipeline            pipeLine() const;
    Qtr3dAssets        *context();
    Qtr3d::ShaderType   shader() const;
    void                setFaceOrientation(Qtr3d::FaceOrientation orientation);
    Qtr3d::FaceOrientation faceOrientation() const;

    void            setDefaultColor(const QColor &c);
    Qtr3d::Material &material();
    const Qtr3d::Material &material() const;

    void             setRenderOption(RenderOption o, bool enabled = true);
    void             setRenderOptions(int optionMask);
    RenderOptions    renderOptions() const;
    bool             hasRenderOption(RenderOption o) const;

    virtual QVector3D minValues() const; // lowest xyz
    virtual QVector3D maxValues() const; // max xyz
    virtual QVector3D center() const;
    virtual double    radius() const;

    // BufferState Iteration
    virtual void registerBufferState(Qtr3dGeometryState *s);
    inline const Qtr3dGeometryBufferStates &bufferStates() const { return mBufferStates; }

    void           setParentBuffer(Qtr3dGeometry *parentBuffer);
    Qtr3dGeometry *parentBuffer() const;

protected:
    void analyze(const QVector3D &v);
    void setShader(Qtr3d::ShaderType t);

private slots:
    void stateDestroyed(QObject *state);

private:
    Pipeline                    mPipeline;
    Qtr3dAssets                *mContext;
    Qtr3dGeometry              *mParentBuffer;

    Qtr3d::ShaderType           mShader;
    Qtr3d::FaceOrientation      mFaceOrientation;
    RenderOptions               mRenderOptions;
    Qtr3dGeometryBufferStates   mBufferStates;

    Qtr3d::Material             mMaterial;

    // Axis-Aligned-Bounding-Box
    QVector3D                   mMin;
    QVector3D                   mMax;
};

typedef QList<Qtr3dGeometry> Qtr3dGeometryBuffers;

#endif // QTR3DGEOMETRY_H
