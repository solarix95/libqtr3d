#include "qtr3dvertexmesh.h"
#include "qtr3dshader.h"
#include <QVector4D>

//-------------------------------------------------------------------------------------------------
Qtr3dVertexMesh::Qtr3dVertexMesh(Type meshType)
    : Qtr3dGeometryBuffer()
    , mVertexBufferId(0)
    , mElementBufferId(0)
    , mVertexCount(0)
{
    startMesh(meshType);
}

//-------------------------------------------------------------------------------------------------
Qtr3dVertexMesh::~Qtr3dVertexMesh()
{
    reset();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMesh::setMeshType(Type t)
{
    mMeshType = t;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMesh::reset()
{
    // TODO: release mVertexBufferId/mElementBufferId
    mVertexBufferId  = 0;
    mElementBufferId = 0;
    mVertexCount     = 0;
    mVertexes.clear();
    mIndexes.clear();
    mNormals.clear();

    startMesh(Unknown);
}

//-------------------------------------------------------------------------------------------------
Qtr3dVertexMesh *Qtr3dVertexMesh::startMesh(Qtr3dVertexMesh::Type meshType, Qtr3dGeometryBuffer::FaceOrientation orientation)
{
    mMeshType          = meshType;
    setFaceOrientation(orientation);

    return this;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMesh::endMesh(bool doTrim)
{
    if (mMeshType == Unknown || mVertexes.isEmpty())
        return;

    // Transfer Vertex Order to Graphic-Memory
    if (mIndexes.isEmpty()) {
        for (int i=0; i<mVertexes.count(); i++) {
            mIndexes << (GLuint)i;
        }
    }

    // Auto-Normal
    if (mMeshType == Triangle || mMeshType == Quad) {
        for (int i=0; i<mIndexes.count();) {
            if (mVertexes.at(mIndexes[i]).n.isNull())
                calculateFaceNormal(mIndexes[i], mIndexes[i+1], mIndexes[i+2]);
            i += (mMeshType == Triangle ? 3 : 4);
        }
    }

    // Transfer Modeldata to Graphic-Memory
    mVertexBufferId  = Qtr3dShader::makeBO(mVertexes.data(),mVertexes.count() * sizeof(Qtr3dColoredVertex));
    mElementBufferId = Qtr3dShader::makeBO(mIndexes.data(),mIndexes.count() * sizeof(GLuint), GL_ELEMENT_ARRAY_BUFFER);
    mVertexCount     = mIndexes.count(); // ready to trim "mIndexes"

    if (doTrim)
        trim();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMesh::trim()
{
    mVertexes.clear();
    mNormals.clear();
    mIndexes.clear();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMesh::addVertex(const QVector3D &v)
{
    addVertex(v,defaultColor());
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
    addVertex(v,n,defaultColor());
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
void Qtr3dVertexMesh::addQuad(const QVector3D &p1, const QVector3D &p2, const QVector3D &p3, const QVector3D &p4, const QVector3D &n)
{
    addVertex(p1,n);
    addVertex(p2,n);
    addVertex(p3,n);

    addVertex(p1,n);
    addVertex(p3,n);
    addVertex(p4,n);
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

//-------------------------------------------------------------------------------------------------
int Qtr3dVertexMesh::vertexListCount() const
{
    return mVertexes.count();
}

//-------------------------------------------------------------------------------------------------
const Qtr3dColoredVertex &Qtr3dVertexMesh::vertex(int i) const
{
    Q_ASSERT(i >= 0);
    Q_ASSERT(i < mVertexes.count());
    return mVertexes[i];
}

//-------------------------------------------------------------------------------------------------
GLenum Qtr3dVertexMesh::bufferType() const
{
    switch (mMeshType) {
    case Unknown: Q_ASSERT(0); return 0; break;
    case Dot:      return GL_POINTS;    break;
    case Line:     return GL_LINES;     break;
    case Triangle: return GL_TRIANGLES; break;
    case Quad:     return GL_QUADS; break;
    }
    Q_ASSERT(0);
    return 0;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMesh::calculateNormal(int vertexIndex)
{
    Q_ASSERT(mMeshType == Triangle || mMeshType == Quad);

    int shapeVertexCount = mMeshType == Triangle ? 3 : 4;

    int relativIndex = vertexIndex % shapeVertexCount;
    int shapeIndex   = vertexIndex / shapeVertexCount;


    // https://stackoverflow.com/questions/19350792/calculate-normal-of-a-single-triangle-in-3d-space
    QVector3D v01(mVertexes.at(vertexIndex+1).p.toQVector() -
                  mVertexes.at(vertexIndex+0).p.toQVector());

    QVector3D v02(mVertexes.at(vertexIndex+2).p.toQVector() -
                  mVertexes.at(vertexIndex+0).p.toQVector());

    /*
    switch(relativIndex) {
    case 0: {
        v01 = mVertexes.at(shapeIndex+1).p.toQVector() - mVertexes.at(shapeIndex+0).p.toQVector();
        v02 = mVertexes.at(shapeIndex+2).p.toQVector() - mVertexes.at(shapeIndex+0).p.toQVector();
    } break;
    case 1: {
        v01 = mVertexes.at(shapeIndex+1).p.toQVector() - mVertexes.at(shapeIndex+0).p.toQVector();
        v02 = mVertexes.at(shapeIndex+2).p.toQVector() - mVertexes.at(shapeIndex+0).p.toQVector();
    } break;
    }
    */

    QVector3D normal = QVector3D::crossProduct(v01, v02).normalized();
    mVertexes[vertexIndex+0].n = normal;
    mVertexes[vertexIndex+1].n = normal;
    mVertexes[vertexIndex+2].n = normal;

    /*
    double l1 = mVertexes.at(shapeIndex+0).p.toQVector().distanceToPoint(center());
    double l2 = (mVertexes.at(shapeIndex+0).p.toQVector() + normal).distanceToPoint(center());

    mVertexes[vertexIndex].n = (l2 > l1) ? normal : -normal;
    */

}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMesh::calculateFaceNormal(int vi1, int vi2, int vi3)
{
    QVector3D v01(mVertexes.at(vi2).p.toQVector() -
                  mVertexes.at(vi1).p.toQVector());

    QVector3D v02(mVertexes.at(vi3).p.toQVector() -
                  mVertexes.at(vi1).p.toQVector());

    QVector3D normal = QVector3D::crossProduct(v01, v02).normalized();
    mVertexes[vi1].n = normal;
    mVertexes[vi2].n = normal;
    mVertexes[vi3].n = normal;

}

//-------------------------------------------------------------------------------------------------
Qtr3dVertexMesh::Type Qtr3dVertexMesh::meshType() const
{
    return mMeshType;
}

