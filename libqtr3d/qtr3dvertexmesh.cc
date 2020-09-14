#include "qtr3dvertexmesh.h"
#include "qtr3dshader.h"

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

    mDefaultColor    = QColor();
    startMesh(Unknown);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMesh::startMesh(Qtr3dVertexMesh::Type meshType)
{
    mMeshType        = meshType;
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
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMesh::addVertex(const Qtr3dColoredVertex &v)
{
    Q_ASSERT(mVertexBufferId == 0);
    mVertexes << v;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMesh::addIndex(int i)
{
    mIndexes << (GLuint)i; // TODO...
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
Qtr3dVertexMesh::Type Qtr3dVertexMesh::meshType() const
{
    return mMeshType;
}
