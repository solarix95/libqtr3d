#include <QDebug>
#include <QVector4D>
#include "qtr3dmesh.h"
#include "shaders/qtr3dshader.h"


//-------------------------------------------------------------------------------------------------
Qtr3dMesh::Qtr3dMesh(Qtr3dAssets *parent)
    : Qtr3dGeometry(parent)
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

    Qtr3dShader::releaseBO(mVerticesBufferId);
    Qtr3dShader::releaseBO(mElementBufferId);
    Qtr3dShader::releaseBO(mElementBufferId);
    mVertexCount       = 0;
    Qtr3dShader::releaseBO(mColorBufferId);
    Qtr3dShader::releaseBO(mTexcoorBufferId);

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
    if (mMeshType == Qtr3d::Triangle || mMeshType == Qtr3d::Quad) {
        int numFaces = (mMeshType == Qtr3d::Triangle) ? 3 : 4;
        for (int i=0; i<mIndexes.count();) {
            // qDebug() << i << mIndexes[i] << mVertices.at(mIndexes[i]).n.toQVector();
            if (mVertices.at(mIndexes[i]).n.isNull())
                calculateFaceNormal(mIndexes[i], mIndexes[i+1], mIndexes[numFaces-1]);
            else
                qDebug() << "Skipped";
            i += numFaces;
        }
    }
    */

    if (mMeshType == Qtr3d::Triangle || mMeshType == Qtr3d::Quad) {
        int numFaces = (mMeshType == Qtr3d::Triangle) ? 3 : 4;
        for (int i=0; i<mIndexes.count();) {
            // qDebug() << i << mIndexes[i] << mVertices.at(mIndexes[i]).n.toQVector();
            if (mVertices.at(mIndexes[i]).n.isNull() || mVertices.at(mIndexes[i+1]).n.isNull() || mVertices.at(mIndexes[i+numFaces-1]).n.isNull())
                calculateFaceNormal(mIndexes[i], mIndexes[i+1], mIndexes[i+numFaces-1]);
            i += numFaces;
        }
    }

    initMeshSkeleton();
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
    mVertices << v.toVertex();
    mColors   << Qtr3dColor(v.c.toQVector());
    analyze(mVertices.last().p.toQVector());
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMesh::addQuad(const QVector3D &p1, const QVector3D &p2, const QVector3D &p3, const QVector3D &p4, const QVector3D &n)
{
    switch(mMeshType) {
    case Qtr3d::Triangle: {
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
    } break;
    case Qtr3d::Quad: {
        addVertex(p1,n);
        addVertex(p2,n);
        addVertex(p3,n);
        addVertex(p4,n);

        mTextureCoords << Qtr3dTexCoord(0,0);
        mTextureCoords << Qtr3dTexCoord(0,1);
        mTextureCoords << Qtr3dTexCoord(1,1);
        mTextureCoords << Qtr3dTexCoord(1,0);

    } break;
    default:
        qWarning() << "Qtr3dMesh::addQuad: invalid Mesh Type";
    }
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMesh::addQuad(const QVector3D &p1, const QVector3D &p2, const QVector3D &p3, const QVector3D &p4, const QColor &c, const QVector3D &n)
{
    switch(mMeshType) {
    case Qtr3d::Triangle: {
        addVertex(p1,n,c);
        addVertex(p2,n,c);
        addVertex(p3,n,c);

        addVertex(p1,n,c);
        addVertex(p3,n,c);
        addVertex(p4,n,c);
    } break;
    case Qtr3d::Quad: {
        addVertex(p1,n,c);
        addVertex(p2,n,c);
        addVertex(p3,n,c);
        addVertex(p4,n,c);
    } break;
    default:
        qWarning() << "Qtr3dMesh::addQuad: invalid Mesh Type";
    }
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMesh::addBone(const Bone &bone)
{
    if (bone.name.isEmpty() || bone.weights.isEmpty()) {
        qWarning() << "Qtr3dMesh::addBone: invalid bone struct";
        return;
    }
    mBones << bone;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMesh::addNormal(const QVector3D &n)
{
    mNormals << n;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMesh::updateBone(int vi, const Qtr3dVector &bi, const Qtr3dVector &bw)
{
    if (vi < 0 || vi >= mVertices.count())
        return;
    mVertices[vi].bi = bi;
    mVertices[vi].bw = bw;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMesh::addIndex(int vi, int ni)
{
    if (vi < mVertices.count() && ni >= 0 && ni < mNormals.count()) {
        if (!mVertices[vi].n.isNull()) {
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
    if (img.isNull())
        return;

    if (mTexture)
        delete mTexture;

    mTexture = new Qtr3dTexture(img.mirrored());
    // mTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    // mTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    mTexture->setMagnificationFilter(QOpenGLTexture::Nearest);
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dMesh::hasTexture() const
{
    return (mTexture != nullptr) || (textureId() > 0);
}

//-------------------------------------------------------------------------------------------------
int Qtr3dMesh::verticesCount() const
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
const QList<Qtr3dMesh::Bone> &Qtr3dMesh::bones() const
{
    return mBones;
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
QVector<QMatrix4x4> Qtr3dMesh::animatedSkeleton() const
{
    QVector<QMatrix4x4> ret;
    for (int i=0; i<mBones.count(); i++) {
        QMatrix4x4 boneTranform;
        boneTranform.setToIdentity();
        ret << boneTranform;
    }

    if (ret.isEmpty()) {
        QMatrix4x4 boneTranform;
        boneTranform.setToIdentity();
        ret << boneTranform;
    }

    return ret;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dMesh::initMeshSkeleton()
{
    if (mBones.isEmpty() || mVertices.isEmpty())
        return;

    struct VertexBones {
        QList<int>   boneIndices;
        QList<float> vertexWeights;
    };

    QVector<VertexBones> indices;
    indices.resize(mVertices.count()); // one Bone-Info per vertex

    for (int boneIndex=0; boneIndex < mBones.count(); boneIndex++) {
        for (const auto &boneWeight : mBones[boneIndex].weights) {
            if (boneWeight.weight <= 0.01) // Ignore bones with less than 1% impact. Why does this exist??!
                continue;
            indices[boneWeight.vertexIndex].boneIndices   << boneIndex;
            indices[boneWeight.vertexIndex].vertexWeights << boneWeight.weight;
        }
    }

    Q_ASSERT(indices.count() == mVertices.count());
    for (int vertexIndex=0; vertexIndex < indices.count(); vertexIndex++) {

        float totalWeight = 0;
        for (auto w: indices[vertexIndex].vertexWeights)
            totalWeight += w;
        float normalizeFactor = 1/totalWeight;

        for (int wi=0; wi < 3; wi++) {
            if (wi > indices[vertexIndex].boneIndices.count()-1) {
                mVertices[vertexIndex].bi[wi] = -1;
                continue;
            }
            mVertices[vertexIndex].bi[wi] = indices[vertexIndex].boneIndices[wi];
            mVertices[vertexIndex].bw[wi] = indices[vertexIndex].vertexWeights[wi] * normalizeFactor;
        }
    }
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

    // int shapeVertexCount = mMeshType == Qtr3d::Triangle ? 3 : 4;
    // int relativIndex = vertexIndex % shapeVertexCount;
    // int shapeIndex   = vertexIndex / shapeVertexCount;


    // https://stackoverflow.com/questions/19350792/calculate-normal-of-a-single-triangle-in-3d-space
    /*
    QVector3D v01(mVertices.at(vertexIndex+1).p.toQVector() -
                  mVertices.at(vertexIndex+0).p.toQVector());

    QVector3D v02(mVertices.at(vertexIndex+2).p.toQVector() -
                  mVertices.at(vertexIndex+0).p.toQVector());
    */

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

    // QVector3D normal = QVector3D::crossProduct(v01, v02).normalized();

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

    // TODO: Refactor
    mVertices[vi1].n = normal;
    mVertices[vi2].n = normal;
    mVertices[vi3].n = normal;

}

//-------------------------------------------------------------------------------------------------
Qtr3d::MeshType Qtr3dMesh::meshType() const
{
    return mMeshType;
}

