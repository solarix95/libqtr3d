#include "qtr3dcontext.h"
#include "qtr3dgeometrybuffer.h"

//-------------------------------------------------------------------------------------------------
Qtr3dGeometryBuffer::Qtr3dGeometryBuffer(Qtr3dContext *parent)
    : QObject(parent)
    , mContext(parent)
    , mParentBuffer(nullptr)
    , mShader(Qtr3d::PlainShader)
    , mFaceOrientation(Qtr3d::DefaultOrientation)
    , mBlending(false)
{
    mMin = QVector3D(  std::numeric_limits<double>::max(),
                       std::numeric_limits<double>::max(),
                       std::numeric_limits<double>::max() );
    mMax = QVector3D( -std::numeric_limits<double>::max(),
                      -std::numeric_limits<double>::max(),
                      -std::numeric_limits<double>::max());
}

//-------------------------------------------------------------------------------------------------
Qtr3dGeometryBuffer::~Qtr3dGeometryBuffer()
{
    qDeleteAll(mBufferStates);
}

//-------------------------------------------------------------------------------------------------
Qtr3dContext *Qtr3dGeometryBuffer::context()
{
    return mContext;
}

//-------------------------------------------------------------------------------------------------
Qtr3d::ShaderType Qtr3dGeometryBuffer::shader() const
{
    return mShader;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dGeometryBuffer::setFaceOrientation(Qtr3d::FaceOrientation orientation)
{
    mFaceOrientation = orientation;
}

//-------------------------------------------------------------------------------------------------
Qtr3d::FaceOrientation Qtr3dGeometryBuffer::faceOrientation() const
{
    Qtr3d::FaceOrientation ret = mFaceOrientation;

    if (ret == Qtr3d::DefaultOrientation && parentBuffer())
        ret = parentBuffer()->faceOrientation();

    if (ret == Qtr3d::DefaultOrientation)
        ret = Qtr3d::ClockWise;

    return ret;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dGeometryBuffer::setDefaultColor(const QColor &c)
{
    material().ambient().mcolor = material().diffuse().mcolor = material().specular().mcolor = c;
}

//-------------------------------------------------------------------------------------------------
Qtr3d::Material &Qtr3dGeometryBuffer::material()
{
    return mMaterial;
}

//-------------------------------------------------------------------------------------------------
const Qtr3d::Material &Qtr3dGeometryBuffer::material() const
{
    return mMaterial;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dGeometryBuffer::setBlendingEnabled(bool enabled)
{
    mBlending = enabled;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dGeometryBuffer::blending() const
{
    return mBlending;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dGeometryBuffer::setParentBuffer(Qtr3dGeometryBuffer *buffer)
{
    mParentBuffer = buffer;
}

//-------------------------------------------------------------------------------------------------
Qtr3dGeometryBuffer *Qtr3dGeometryBuffer::parentBuffer() const
{
    return mParentBuffer;
}

//-------------------------------------------------------------------------------------------------
QVector3D Qtr3dGeometryBuffer::minValues() const
{
    return mMin;
}

//-------------------------------------------------------------------------------------------------
QVector3D Qtr3dGeometryBuffer::maxValues() const
{
    return mMax;
}

//-------------------------------------------------------------------------------------------------
QVector3D Qtr3dGeometryBuffer::center() const
{
    return QVector3D(
                (maxValues().x() + minValues().x())/2,
                (maxValues().y() + minValues().y())/2,
                (maxValues().z() + minValues().z())/2
                );
}

//-------------------------------------------------------------------------------------------------
double Qtr3dGeometryBuffer::radius() const
{
    return qMax(maxValues().x() - minValues().x(),qMax(maxValues().y() - minValues().y(),maxValues().z() - minValues().z()))/2.0;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dGeometryBuffer::registerBufferState(Qtr3dGeometryBufferState *s)
{
    Q_ASSERT(s);
    Q_ASSERT(!mBufferStates.contains(s));
    mBufferStates << s;
    connect(s, &Qtr3dGeometryBufferState::destroyed, this, &Qtr3dGeometryBuffer::stateDestroyed);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dGeometryBuffer::analyze(const QVector3D &v)
{
    if (v.x() > mMax.x())
        mMax.setX(v.x());

    if (v.y() > mMax.y())
        mMax.setY(v.y());

    if (v.z() > mMax.z())
        mMax.setZ(v.z());

    if (v.x() < mMin.x())
        mMin.setX(v.x());

    if (v.y() < mMin.y())
        mMin.setY(v.y());

    if (v.z() < mMin.z())
        mMin.setZ(v.z());
}

//-------------------------------------------------------------------------------------------------
void Qtr3dGeometryBuffer::setShader(Qtr3d::ShaderType t)
{
    mShader = t;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dGeometryBuffer::stateDestroyed(QObject *state)
{
    mBufferStates.removeOne((Qtr3dGeometryBufferState*)state);
}

