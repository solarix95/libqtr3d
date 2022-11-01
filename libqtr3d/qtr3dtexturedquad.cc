#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include "qtr3dtexturedquad.h"
#include "qtr3dtexturefactory.h"
#include "qtr3dtypes.h"

// Element data - just draw all vertices in order
GLushort elementData[] = {
    0,  1,  2,  3};

// Simple helper to make a single buffer object.
GLuint makeBO( QOpenGLFunctions *f, GLenum type, void* data, GLsizei size, int accessFlags ) {
    GLuint bo;
    f->glGenBuffers( 1, &bo );
    f->glBindBuffer( type, bo );
    f->glBufferData( type, size, data, accessFlags );
    return( bo );
}

//-------------------------------------------------------------------------------------------------
Qtr3dTexturedQuad::Qtr3dTexturedQuad(Qtr3dTextureFactory *textures)
    : mTextures(textures)
    , mTexture(nullptr)
    , mVertexBufferId(0)
    , mElementBufferId(0)
{
}

//-------------------------------------------------------------------------------------------------
Qtr3dTexturedQuad::Qtr3dTexturedQuad(Qtr3dTextureFactory *textures, const QString &textureName)
    : mTextures(textures)
    , mTexture(nullptr)
    , mVertexBufferId(0)
    , mElementBufferId(0)
    , mTextureName(textureName)
{
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedQuad::startMesh(const QString &textureName)
{
    destroy();
    if (!textureName.isEmpty())
        mTextureName = textureName;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedQuad::addQuad(const QVector3D &p1, const QVector3D &p2, const QVector3D &p3, const QVector3D &p4)
{
/*
 Qtr3dTexturedVertex vertices[] = {
    { {  1.0,  1.0,  0.0 }, 1.0, {  0.0,  0.0,  1.0 }, 1.0, 0.0 },
    { { -1.0,  1.0,  0.0 }, 1.0, {  0.0,  0.0,  1.0 }, 0.0, 0.0 },
    { { -1.0, -1.0,  0.0 }, 1.0, {  0.0,  0.0,  1.0 }, 0.0, 1.0 },
    { {  1.0, -1.0,  0.0 }, 1.0, {  0.0,  0.0,  1.0 }, 1.0, 1.0 }
};
*/
    mVertices << Qtr3dTexturedVertex(p1,{0,0,1},1,0);
    mVertices << Qtr3dTexturedVertex(p2,{0,0,1},0,0);
    mVertices << Qtr3dTexturedVertex(p3,{0,0,1},0,1);
    mVertices << Qtr3dTexturedVertex(p4,{0,0,1},1,1);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedQuad::endMesh()
{
    init(mTextureName);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedQuad::reset()
{
    destroy();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedQuad::init(const QString &fname)
{
    releaseGlIds();
    mTexture = mTextures->texture(fname);
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    mVertexBufferId  = makeBO(f,GL_ARRAY_BUFFER,mVertices.data(),mVertices.count() * sizeof(Qtr3dTexturedVertex),GL_STATIC_DRAW);

    QVector<GLushort> elementOrder;
    for (int i=0; i<mVertices.count(); i++)
        elementOrder << i;

    mElementBufferId = makeBO(f, GL_ELEMENT_ARRAY_BUFFER, elementOrder.data(), elementOrder.count() * sizeof( GLushort ), GL_STATIC_DRAW);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedQuad::destroy()
{
    releaseGlIds();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedQuad::calculateNormals()
/*

  p1 ------> p2
  |          |
  |          |
  |          |
  \/         |
  p4------- p3

*/

{
    if (mVertices.count() % 4 != 0)
        return;
    for (int quad=0; quad < (mVertices.count()/4); quad++) {
        int startIndex = quad*4;

        QVector3D p1 = mVertices[startIndex+0].p.toQVector();
        QVector3D p2 = mVertices[startIndex+1].p.toQVector();
        QVector3D p3 = mVertices[startIndex+2].p.toQVector();
        QVector3D p4 = mVertices[startIndex+3].p.toQVector();

        // Normalvector for p1
        QVector3D v1 = p2 - p1;
        QVector3D v2 = p4 - p1;
        mVertices[startIndex].n   = QVector3D(0,0,1);
        mVertices[startIndex+1].n = QVector3D(0,0,1);
        mVertices[startIndex+2].n = QVector3D(0,0,1);
        mVertices[startIndex+3].n = QVector3D(0,0,1);

        return;
        mVertices[startIndex].n = -1* QVector3D::crossProduct(v1,v2).normalized();

        // Normalvector for p2
        v1 = p1 - p2;
        v2 = p3 - p2;
        mVertices[startIndex+1].n = QVector3D::crossProduct(v2,v1);

        // Normalvector for p3
        v1 = p2 - p3;
        v2 = p4 - p3;
        mVertices[startIndex+2].n = QVector3D::crossProduct(v1,v2);

        // Normalvector for p4
        v1 = p1 - p4;
        v2 = p3 - p4;
        mVertices[startIndex+3].n = QVector3D::crossProduct(v1,v2);
    }
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedQuad::releaseGlIds()
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





