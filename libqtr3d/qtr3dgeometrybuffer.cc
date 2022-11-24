#include "qtr3dgeometrybuffer.h"

//-------------------------------------------------------------------------------------------------
Qtr3dGeometryBuffer::Qtr3dGeometryBuffer()
    : mFaceOrientation(DefaultOrientation)
    , mParent(nullptr)
{
    mMin = QVector3D( std::numeric_limits<double>::max(),
                      std::numeric_limits<double>::max(),
                      std::numeric_limits<double>::max() );
    mMax = QVector3D( std::numeric_limits<double>::min(),
                      std::numeric_limits<double>::min(),
                      std::numeric_limits<double>::min());
}

//-------------------------------------------------------------------------------------------------
Qtr3dGeometryBuffer::~Qtr3dGeometryBuffer()
{
    qDeleteAll(mBufferStates);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dGeometryBuffer::setFaceOrientation(FaceOrientation orientation)
{
    mFaceOrientation = orientation;
}

//-------------------------------------------------------------------------------------------------
Qtr3dGeometryBuffer::FaceOrientation Qtr3dGeometryBuffer::faceOrientation() const
{
    FaceOrientation ret = mFaceOrientation;

    if (ret == DefaultOrientation && parentBuffer())
        ret = parentBuffer()->faceOrientation();

    if (ret == DefaultOrientation)
        ret = ClockWise;

    return ret;
}

//-------------------------------------------------------------------------------------------------
Qtr3d::Material &Qtr3dGeometryBuffer::material()
{
    return mMaterial;
}

//-------------------------------------------------------------------------------------------------
const Qtr3d::Material &Qtr3dGeometryBuffer::cMaterial() const
{
    return mMaterial;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dGeometryBuffer::setParentBuffer(Qtr3dGeometryBuffer *buffer)
{
    mParent = buffer;
}

//-------------------------------------------------------------------------------------------------
Qtr3dGeometryBuffer *Qtr3dGeometryBuffer::parentBuffer() const
{
    return mParent;
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
    return qMax(maxValues().x() - minValues().x(),qMax(maxValues().y() - minValues().y(),maxValues().z() - minValues().z()));
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
void Qtr3dGeometryBuffer::stateDestroyed(QObject *state)
{
    mBufferStates.removeOne((Qtr3dGeometryBufferState*)state);
}

