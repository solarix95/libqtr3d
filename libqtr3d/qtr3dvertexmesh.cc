#include "qtr3dvertexmesh.h"
#include "qtr3dshader.h"
#include <QVector4D>

//-------------------------------------------------------------------------------------------------
Qtr3dVertexMesh::Qtr3dVertexMesh(Type meshType)
    : Qtr3dGeometryBuffer()
    , mVertexBufferId(0)
    , mElementBufferId(0)
    , mDefaultColor(Qt::white)
{
    startMesh(meshType);
}

//-------------------------------------------------------------------------------------------------
Qtr3dVertexMesh::~Qtr3dVertexMesh()
{
    reset();
}

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
    mVertexes.clear();
    mIndexes.clear();
    mNormals.clear();

    mDefaultColor    = QColor();
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
void Qtr3dVertexMesh::endMesh()
{
    if (mMeshType == Unknown || mVertexes.isEmpty())
        return;

    if (mMeshType == Triangle || mMeshType == Quad) {
        for (int i=0; i<mVertexes.count(); i++) {
            if (i != 4)
                continue;
            if (mVertexes.at(i).n.isNull()) {
                calculateNormal(i);
            }
        }
    }


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


    QVector3D v01(mVertexes.at(shapeIndex+1).p.toQVector() -
                  mVertexes.at(shapeIndex+0).p.toQVector());

    QVector3D v02(mVertexes.at(shapeIndex+2).p.toQVector() -
                  mVertexes.at(shapeIndex+0).p.toQVector());

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

    QVector3D normal = QVector3D::crossProduct(v01, v02).normalized();
    mVertexes[vertexIndex].n = normal;

    /*
    double l1 = mVertexes.at(shapeIndex+0).p.toQVector().distanceToPoint(center());
    double l2 = (mVertexes.at(shapeIndex+0).p.toQVector() + normal).distanceToPoint(center());

    mVertexes[vertexIndex].n = (l2 > l1) ? normal : -normal;
    */

}

//-------------------------------------------------------------------------------------------------
Qtr3dVertexMesh::Type Qtr3dVertexMesh::meshType() const
{
    return mMeshType;
}

