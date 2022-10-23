#include "qtr3dvertexmesh.h"
#include "qtr3dshader.h"
#include <QVector4D>

//-------------------------------------------------------------------------------------------------
Qtr3dVertexMesh::Qtr3dVertexMesh(Type meshType)
    : Qtr3dGeometryBuffer()
    , mVertexBufferId(0)
    , mElementBufferId(0)
{
    startMesh(meshType);
}

//-------------------------------------------------------------------------------------------------
Qtr3dVertexMesh::~Qtr3dVertexMesh()
{
    reset();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMesh::reset()
{
    // TODO: release mVertexBufferId/mElementBufferId
    mVertexBufferId  = 0;
    mElementBufferId = 0;
    mVertexes.clear();
    mIndexes.clear();
    mNormals.clear();

    mDefaultColor    = QColor();
    startMesh(Unknown);
}

//-------------------------------------------------------------------------------------------------
Qtr3dVertexMesh *Qtr3dVertexMesh::startMesh(Qtr3dVertexMesh::Type meshType)
{
    mMeshType        = meshType;
    mMin = QVector3D( std::numeric_limits<double>::max(),
                      std::numeric_limits<double>::max(),
                      std::numeric_limits<double>::max() );
    mMax = QVector3D( std::numeric_limits<double>::min(),
                      std::numeric_limits<double>::min(),
                      std::numeric_limits<double>::min());
    return this;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMesh::endMesh()
{
    if (mMeshType == Unknown || mVertexes.isEmpty())
        return;

    // Transfer Modeldata to Graphic-Memory
    mVertexBufferId = Qtr3dShader::makeBO(mVertexes.data(),mVertexes.count() * sizeof(Qtr3dColoredVertex));

    // Transfer Vertex Order to Graphic-Memory
    if (mIndexes.isEmpty()) {
        for (int i=0; i<mVertexes.count(); i++) {
            mIndexes << (GLuint)i;
        }
    }

    mNormals.clear();
    mElementBufferId = Qtr3dShader::makeBO(mIndexes.data(),mIndexes.count() * sizeof(GLuint), GL_ELEMENT_ARRAY_BUFFER);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMesh::setDefaultColor(const QColor &c)
{
    mDefaultColor = c;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMesh::addVertex(const QVector3D &v)
{
    addVertex(v,mDefaultColor);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMesh::addVertex(const QVector3D &v, const QColor &c)
{
    Q_ASSERT(mVertexBufferId == 0);
    mVertexes << Qtr3dColoredVertex(v,c);
    analyze(v);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMesh::addVertex(const QVector3D &v, const QVector3D &n)
{
    addVertex(v,n,mDefaultColor);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMesh::addVertex(const QVector3D &v, const QVector3D &n, const QColor &c)
{
    Q_ASSERT(mVertexBufferId == 0);
    mVertexes << Qtr3dColoredVertex(v,n.normalized(),c);
    analyze(v);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMesh::addVertex(const Qtr3dColoredVertex &v)
{
    Q_ASSERT(mVertexBufferId == 0);
    mVertexes << v;
    analyze(QVector3D(v.p.x, v.p.y, v.p.z));
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMesh::addNormal(const QVector3D &n)
{
    mNormals << n;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMesh::addIndex(int vi,int ti, int ni)
{
    mIndexes << (GLuint)vi;

    if (vi < mVertexes.count() && ni >= 0 && ni < mNormals.count())
        mVertexes[vi].n = mNormals[ni];
}

QVector3D Qtr3dVertexMesh::minValues() const
{
    return mMin;
}

QVector3D Qtr3dVertexMesh::maxValues() const
{
    return mMax;
}

//-------------------------------------------------------------------------------------------------
QVector3D Qtr3dVertexMesh::center() const
{
    return QVector3D(
                (mMax.x() + mMin.x())/2,
                (mMax.y() + mMin.y())/2,
                (mMax.z() + mMin.z())/2
                );
}

//-------------------------------------------------------------------------------------------------
double Qtr3dVertexMesh::radius() const
{
    return qMax(mMax.x() - mMin.x(),qMax(mMax.y() - mMin.y(),mMax.z() - mMin.z()));
}

//-------------------------------------------------------------------------------------------------
GLenum Qtr3dVertexMesh::bufferType() const
{
    switch (mMeshType) {
    case Unknown: Q_ASSERT(0); return 0; break;
    case Dot:      return GL_POINTS;    break;
    case Line:     return GL_LINES;     break;
    case Triangle: return GL_TRIANGLES; break;
    case Quad:     return GL_QUADS;     break;
    }
    Q_ASSERT(0);
    return 0;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMesh::analyze(const QVector3D &v)
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
Qtr3dVertexMesh::Type Qtr3dVertexMesh::meshType() const
{
    return mMeshType;
}
