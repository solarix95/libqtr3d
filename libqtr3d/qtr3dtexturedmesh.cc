
#include <QOpenGLFunctions>
#include <QOpenGLTexture>

#include "shaders/qtr3dshader.h"
#include "qtr3dtexturefactory.h"
#include "qtr3dtypes.h"
#include "qtr3dtexturedmesh.h"

//-------------------------------------------------------------------------------------------------
Qtr3dTexturedMesh::Qtr3dTexturedMesh(Qtr3dTextureFactory *textures, Qtr3d::MeshType meshType)
    : Qtr3dGeometryBuffer(nullptr)
    , mType(meshType)
    , mTextures(textures)
    , mTexture(nullptr)
    , mVertexBufferId(0)
    , mElementBufferId(0)
{
}

//-------------------------------------------------------------------------------------------------
Qtr3dTexturedMesh::Qtr3dTexturedMesh(Qtr3dTextureFactory *textures, const QString &textureName, Qtr3d::MeshType meshType )
    : Qtr3dGeometryBuffer(nullptr)
    , mType(meshType)
    , mTextures(textures)
    , mTexture(nullptr)
    , mVertexBufferId(0)
    , mElementBufferId(0)
    , mTextureName(textureName)
{
}

//-------------------------------------------------------------------------------------------------
Qtr3d::MeshType Qtr3dTexturedMesh::meshType() const
{
    return mType;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedMesh::startMesh(Qtr3d::MeshType meshType, const QString &textureName)
{
    destroy();
    if (!textureName.isEmpty())
        mTextureName = textureName;
    if (meshType != Qtr3d::UnknownMesh)
        mType = meshType;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedMesh::addVertex(const QVector3D &vertex, Qtr3dScalar u, Qtr3dScalar v, const QVector3D &n)
{
    mVertices << Qtr3dTexturedVertex(vertex,n,u,v);
    analyze(vertex);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedMesh::addQuad(const QVector3D &p1, const QVector3D &p2, const QVector3D &p3, const QVector3D &p4, const QVector3D &n)
{
    addVertex(p1,1,0,n);
    addVertex(p2,0,0,n);
    addVertex(p3,0,1,n);

    addVertex(p1,1,0,n);
    addVertex(p3,0,1,n);
    addVertex(p4,1,1,n);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedMesh::addIndex(int faceIndex)
{
    mIndexes << faceIndex;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedMesh::endMesh()
{
    init(mTextureName);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedMesh::reset()
{
    destroy();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedMesh::init(const QString &fname)
{
    releaseGlIds();
    mTexture         = mTextures->texture(fname);
    mVertexBufferId  = Qtr3dShader::makeBO(mVertices.data(),mVertices.count() * sizeof(Qtr3dTexturedVertex),GL_ARRAY_BUFFER,GL_STATIC_DRAW);

    if (mIndexes.isEmpty()) {
        for (int i=0; i<mVertices.count(); i++)
        mIndexes << i;
    }

    mElementBufferId = Qtr3dShader::makeBO(mIndexes.data(),mIndexes.count() * sizeof( GLuint ), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedMesh::destroy()
{
    releaseGlIds();
}

//-------------------------------------------------------------------------------------------------
// FIXME: to GeometryBuffer
GLenum Qtr3dTexturedMesh::bufferType() const
{
    switch (mType) {
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
void Qtr3dTexturedMesh::releaseGlIds()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    if (mVertexBufferId > 0) {
        f->glDeleteBuffers(1,&mVertexBufferId);
        mVertexBufferId = 0;
    }
    if (mElementBufferId > 0) {
        f->glDeleteBuffers(1,&mElementBufferId);
        mElementBufferId = 0;
    }
    /*
    if (mTextureId > 0)
        f->glDeleteTextures(1,&mTextureId);
    */
    if (mTexture) {
        delete mTexture;
        mTexture = nullptr;
    }
}

