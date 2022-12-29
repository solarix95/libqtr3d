#ifndef QTR3DCOLOREDELEMENTS_H
#define QTR3DCOLOREDELEMENTS_H

#include <QVector>
#include <QObject>
#include "qtr3dgeometrybuffer.h"
#include "qtr3dtypes.h"

class QOpenGLTexture;
class Qtr3dMesh : public Qtr3dGeometryBuffer
{
public:
    Qtr3dMesh(Qtr3dContext *parent);
    virtual ~Qtr3dMesh();

    void              setMeshType(Qtr3d::MeshType t);
    Qtr3d::MeshType   meshType() const;

    void              reset();
    Qtr3dMesh        *startMesh(Qtr3d::MeshType meshType, Qtr3d::FaceOrientation orientation = Qtr3d::DefaultOrientation);
    void              endMesh(bool doTrim = false);
    void              trim(); // free memory by throw away unused data structures

    void addVertex(const QVector3D &v); // .. and use "defaultColor"
    void addVertex(const QVector3D &v, const QColor &c);
    void addVertex(const QVector3D &v, const QVector3D &n);
    void addVertex(const QVector3D &v, const QVector3D &n, const QColor &c);

    void addVertex(const QVector3D &v, float tu, float tv);
    void addVertex(const QVector3D &v, const QVector3D &n, Qtr3dScalar tu, Qtr3dScalar tv);

    void addVertex(const Qtr3dColoredVertex &v);
    void addQuad(const QVector3D &p1, const QVector3D &p2, const QVector3D &p3, const QVector3D &p4, const QVector3D &n  = QVector3D());

    void addNormal(const QVector3D &n);
    void addIndex(int vi, int ni = -1);

    void setTexture(const QImage &img);

    // reader
    int               vertexListCount() const;
    const Qtr3dVertex &vertex(int i) const;

    // Shader Interface
    inline GLuint vertexBufferId() const  { return mVerticesBufferId;  }
    inline GLuint elementBufferId() const { return mElementBufferId; }
    inline int    vertexCount() const     { return mVertexCount > 0 ? mVertexCount : (mIndexes.isEmpty() ? mVertices.count() : mIndexes.count());}
    inline GLuint colorBufferId() const   { return mColorBufferId;  }
    inline GLuint texcoordBufferId() const{ return mTexcoorBufferId;  }
    inline GLuint textureId() const       { return mTexture ? mTexture->textureId():0; }
    GLenum        bufferType() const;

private:
    void selectShader();
    void calculateNormal(int vertexIndex);
    void calculateFaceNormal(int vi1, int vi2, int vi3);

    GLuint mVerticesBufferId;
    GLuint mElementBufferId;
    int    mVertexCount;

    GLuint mColorBufferId;
    GLuint mTexcoorBufferId;

    Qtr3d::MeshType          mMeshType;

    QVector<Qtr3dVertex>     mVertices;
    QVector<GLuint>          mIndexes;

    QList<QVector3D>         mNormals;

    QVector<Qtr3dColor>      mColors;
    QVector<Qtr3dTexCoord>   mTextureCoords;

    QOpenGLTexture          *mTexture;
};


typedef QList<Qtr3dMesh*> Qtr3dVertexMeshes;

#endif // QTR3DCOLOREDELEMENTS_H
