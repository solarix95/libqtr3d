#include "qtr3dcontext.h"
#include "qtr3dgeometry.h"

//-------------------------------------------------------------------------------------------------
Qtr3dGeometry::Qtr3dGeometry(Qtr3dContext *parent)
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
Qtr3dGeometry::~Qtr3dGeometry()
{
    qDeleteAll(mBufferStates);
}

//-------------------------------------------------------------------------------------------------
Qtr3dContext *Qtr3dGeometry::context()
{
    return mContext;
}

//-------------------------------------------------------------------------------------------------
Qtr3d::ShaderType Qtr3dGeometry::shader() const
{
    return mShader;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dGeometry::setFaceOrientation(Qtr3d::FaceOrientation orientation)
{
    mFaceOrientation = orientation;
}

//-------------------------------------------------------------------------------------------------
Qtr3d::FaceOrientation Qtr3dGeometry::faceOrientation() const
{
    Qtr3d::FaceOrientation ret = mFaceOrientation;

    if (ret == Qtr3d::DefaultOrientation && parentBuffer())
        ret = parentBuffer()->faceOrientation();

    if (ret == Qtr3d::DefaultOrientation)
        ret = Qtr3d::ClockWise;

    return ret;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dGeometry::setDefaultColor(const QColor &c)
{
    material().ambient().mcolor = material().diffuse().mcolor = material().specular().mcolor = c;
}

//-------------------------------------------------------------------------------------------------
Qtr3d::Material &Qtr3dGeometry::material()
{
    return mMaterial;
}

//-------------------------------------------------------------------------------------------------
const Qtr3d::Material &Qtr3dGeometry::material() const
{
    return mMaterial;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dGeometry::setBlendingEnabled(bool enabled)
{
    mBlending = enabled;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dGeometry::blending() const
{
    return mBlending;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dGeometry::setParentBuffer(Qtr3dGeometry *buffer)
{
    mParentBuffer = buffer;
}

//-------------------------------------------------------------------------------------------------
Qtr3dGeometry *Qtr3dGeometry::parentBuffer() const
{
    return mParentBuffer;
}

//-------------------------------------------------------------------------------------------------
QVector3D Qtr3dGeometry::minValues() const
{
    return mMin;
}

//-------------------------------------------------------------------------------------------------
QVector3D Qtr3dGeometry::maxValues() const
{
    return mMax;
}

//-------------------------------------------------------------------------------------------------
QVector3D Qtr3dGeometry::center() const
{
    return QVector3D(
                (maxValues().x() + minValues().x())/2,
                (maxValues().y() + minValues().y())/2,
                (maxValues().z() + minValues().z())/2
                );
}

//-------------------------------------------------------------------------------------------------
double Qtr3dGeometry::radius() const
{
    return qMax(maxValues().x() - minValues().x(),qMax(maxValues().y() - minValues().y(),maxValues().z() - minValues().z()))/2.0;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dGeometry::registerBufferState(Qtr3dGeometryState *s)
{
    Q_ASSERT(s);
    Q_ASSERT(!mBufferStates.contains(s));
    mBufferStates << s;
    connect(s, &Qtr3dGeometryState::destroyed, this, &Qtr3dGeometry::stateDestroyed);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dGeometry::analyze(const QVector3D &v)
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
void Qtr3dGeometry::setShader(Qtr3d::ShaderType t)
{
    mShader = t;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dGeometry::stateDestroyed(QObject *state)
{
    mBufferStates.removeOne((Qtr3dGeometryState*)state);
}

