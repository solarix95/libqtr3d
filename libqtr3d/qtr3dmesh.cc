#include "qtr3dmesh.h"
#include "shaders/qtr3dshader.h"
#include <QVector4D>

//-------------------------------------------------------------------------------------------------
Qtr3dMesh::Qtr3dMesh(Qtr3dContext *parent)
    : Qtr3dGeometryBuffer(parent)
    , mVerticesBufferId(0)
    , mElementBufferId(0)
    , mVertexCount(0)
    , mColorBufferId(0)
    , mTexcoorBufferId(0)
    , mMeshType(Qtr3d::UnknownMesh)
    , mTexture(nullptr)
{
}

//-------------------------------------------------------------------------------------------------
Qtr3dMesh::~Qtr3dMesh()
{
    reset();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMesh::setMeshType(Qtr3d::MeshType t)
{
    mMeshType = t;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMesh::reset()
{
    // TODO: release mVertexBufferId/mElementBufferId glDeleteBuffers
    mVerticesBufferId  = 0;
    mElementBufferId   = 0;
    mVertexCount       = 0;
    mColorBufferId     = 0;
    mTexcoorBufferId   = 0;
    mVertices.clear();
    mIndexes.clear();
    mNormals.clear();
    mTextureCoords.clear();

    if (mTexture) {
        delete mTexture;
        mTexture = nullptr;
    }
    startMesh(Qtr3d::UnknownMesh);
}

//-------------------------------------------------------------------------------------------------
Qtr3dMesh *Qtr3dMesh::startMesh(Qtr3d::MeshType meshType, Qtr3d::FaceOrientation orientation)
{
    mMeshType          = meshType;
    setFaceOrientation(orientation);
    return this;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMesh::endMesh(bool doTrim)
{
    if (mMeshType == Qtr3d::UnknownMesh || mVertices.isEmpty())
        return;

    // Transfer Vertex Order to Graphic-Memory
    if (mIndexes.isEmpty()) {
        for (int i=0; i<mVertices.count(); i++) {
            mIndexes << (GLuint)i;
        }
    }

    // Auto-Normal
    /*
    if (mMeshType == Triangle || mMeshType == Quad) {
        for (int i=0; i<mIndexes.count();) {
            if (mVertices.at(mIndexes[i]).n.isNull())
                calculateFaceNormal(mIndexes[i], mIndexes[i+1], mIndexes[i+2]);
            i += (mMeshType == Triangle ? 3 : 4);
        }
    }
    */

    selectShader();

    // Transfer Modeldata to Graphic-Memory
    mVerticesBufferId  = Qtr3dShader::makeBO(mVertices.data(),mVertices.count() * sizeof(Qtr3dVertex));
    mElementBufferId   = Qtr3dShader::makeBO(mIndexes.data(),mIndexes.count() * sizeof(GLuint), GL_ELEMENT_ARRAY_BUFFER);
    mVertexCount       = mIndexes.count(); // ready to trim "mIndexes"

    if (mColors.count() > 0) {
        Q_ASSERT(mColors.count() == mVertices.count());
        mColorBufferId = Qtr3dShader::makeBO(mColors.data(),mColors.count() * sizeof(Qtr3dColor));
    }
    if (mTextureCoords.count() > 0) {
        Q_ASSERT(mTextureCoords.count() == mVertices.count());
        mTexcoorBufferId = Qtr3dShader::makeBO(mTextureCoords.data(),mTextureCoords.count() * sizeof(Qtr3dTexCoord));
    }
    if (doTrim)
        trim();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMesh::trim()
{
    mVertices.clear();
    mNormals.clear();
    mIndexes.clear();

    mColors.clear();
    mTextureCoords.clear();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMesh::addVertex(const QVector3D &v)
{
    mVertices << Qtr3dVertex(v);
    analyze(v);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMesh::addVertex(const QVector3D &v, const QColor &c)
{
    mVertices << Qtr3dVertex(v);
    mColors   << Qtr3dColor(c);
    analyze(v);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMesh::addVertex(const QVector3D &v, const QVector3D &n)
{
    mVertices << Qtr3dVertex(v,n);
    analyze(v);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMesh::addVertex(const QVector3D &v, const QVector3D &n, const QColor &c)
{
    mVertices << Qtr3dVertex(v,n);
    mColors   << Qtr3dColor(c);
    analyze(v);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMesh::addVertex(const QVector3D &v, float tu, float tv)
{
    mVertices << Qtr3dVertex(v);
    analyze(v);
    mTextureCoords << Qtr3dTexCoord(tu,tv);
}

void Qtr3dMesh::addVertex(const QVector3D &v, const QVector3D &n, Qtr3dScalar tu, Qtr3dScalar tv)
{
    mVertices      << Qtr3dVertex(v,n);
    mTextureCoords << Qtr3dTexCoord(tu,tv);
    analyze(v);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMesh::addVertex(const Qtr3dColoredVertex &v)
{
    mVertices << Qtr3dVertex(v.p.toQVector(), v.n.toQVector());
    mColors   << Qtr3dColor(v.c.toQVector());
    analyze(mVertices.last().p.toQVector());
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMesh::addQuad(const QVector3D &p1, const QVector3D &p2, const QVector3D &p3, const QVector3D &p4, const QVector3D &n)
{
    addVertex(p1,n);
    addVertex(p2,n);
    addVertex(p3,n);

    addVertex(p1,n);
    addVertex(p3,n);
    addVertex(p4,n);

    mTextureCoords << Qtr3dTexCoord(1,0);
    mTextureCoords << Qtr3dTexCoord(0,0);
    mTextureCoords << Qtr3dTexCoord(0,1);

    mTextureCoords << Qtr3dTexCoord(1,0);
    mTextureCoords << Qtr3dTexCoord(0,1);
    mTextureCoords << Qtr3dTexCoord(1,1);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMesh::addNormal(const QVector3D &n)
{
    mNormals << n;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMesh::addIndex(int vi, int ni)
{
    if (vi < mVertices.count() && ni >= 0 && ni < mNormals.count()) {
        if (!mVertices[vi].n.isNull()) { // TODO: re-using of vertices with different normals?
            // qWarning() << "Qtr3dMesh::addIndex: normal already in use";
            mVertices << mVertices[vi];
            vi = mVertices.count()-1;
        }
        mVertices[vi].n = mNormals[ni];
    }

    mIndexes << (GLuint)vi;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMesh::setTexture(const QImage &img)
{
    if (mTexture)
        delete mTexture;

    mTexture = new Qtr3dTexture(img.mirrored());
    // mTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    // mTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    mTexture->setMagnificationFilter(QOpenGLTexture::Nearest);
}

//-------------------------------------------------------------------------------------------------
int Qtr3dMesh::vertexListCount() const
{
    return mVertices.count();
}

//-------------------------------------------------------------------------------------------------
const Qtr3dVertex &Qtr3dMesh::vertex(int i) const
{
    Q_ASSERT(i >= 0);
    Q_ASSERT(i < mVertices.count());
    return mVertices[i];
}

//-------------------------------------------------------------------------------------------------
GLenum Qtr3dMesh::bufferType() const
{
    switch (mMeshType) {
    case Qtr3d::UnknownMesh: Q_ASSERT(0); return 0; break;
    case Qtr3d::Dot:      return GL_POINTS;    break;
    case Qtr3d::Line:     return GL_LINES;     break;
    case Qtr3d::Triangle: return GL_TRIANGLES; break;
    case Qtr3d::Quad:     return GL_QUADS; break;
    }
    Q_ASSERT(0);
    return 0;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMesh::selectShader()
{
    Qtr3d::ShaderType t = Qtr3d::PlainShader;
    if (!mColors.isEmpty())
        t = Qtr3d::ColoredShader;
    if (!mTextureCoords.isEmpty() && mTexture)
        t = Qtr3d::TexturedShader;

    setShader(t);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMesh::calculateNormal(int vertexIndex)
{
    Q_ASSERT(mMeshType == Qtr3d::Triangle || mMeshType == Qtr3d::Quad);

    int shapeVertexCount = mMeshType == Qtr3d::Triangle ? 3 : 4;

    // int relativIndex = vertexIndex % shapeVertexCount;
    // int shapeIndex   = vertexIndex / shapeVertexCount;


    // https://stackoverflow.com/questions/19350792/calculate-normal-of-a-single-triangle-in-3d-space
    QVector3D v01(mVertices.at(vertexIndex+1).p.toQVector() -
                  mVertices.at(vertexIndex+0).p.toQVector());

    QVector3D v02(mVertices.at(vertexIndex+2).p.toQVector() -
                  mVertices.at(vertexIndex+0).p.toQVector());

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

    Q_ASSERT(0);
    // TODO: Refactor
    // mVertices[vertexIndex+0].n = normal;
    // mVertices[vertexIndex+1].n = normal;
    // mVertices[vertexIndex+2].n = normal;

    /*
    double l1 = mVertexes.at(shapeIndex+0).p.toQVector().distanceToPoint(center());
    double l2 = (mVertexes.at(shapeIndex+0).p.toQVector() + normal).distanceToPoint(center());

    mVertexes[vertexIndex].n = (l2 > l1) ? normal : -normal;
    */

}

//-------------------------------------------------------------------------------------------------
void Qtr3dMesh::calculateFaceNormal(int vi1, int vi2, int vi3)
{
    QVector3D v01(mVertices.at(vi2).p.toQVector() -
                  mVertices.at(vi1).p.toQVector());

    QVector3D v02(mVertices.at(vi3).p.toQVector() -
                  mVertices.at(vi1).p.toQVector());

    QVector3D normal = QVector3D::crossProduct(v01, v02).normalized();

    Q_ASSERT(0);
    // TODO: Refactor
    // mVertices[vi1].n = normal;
    // mVertices[vi2].n = normal;
    // mVertices[vi3].n = normal;

}

//-------------------------------------------------------------------------------------------------
Qtr3d::MeshType Qtr3dMesh::meshType() const
{
    return mMeshType;
}

