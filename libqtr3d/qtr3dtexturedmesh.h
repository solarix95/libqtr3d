#ifndef QTR3DTEXTUREDMESH_H
#define QTR3DTEXTUREDMESH_H


#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QImage>
#include <QString>
#include <QList>
#include <QObject>
#include "qtr3dgeometrybuffer.h"
#include "qtr3dtypes.h"

class Qtr3dGeometryBufferState;
class Qtr3dTextureFactory;

//-------------------------------------------------------------------------------------------------
class Qtr3dTexturedMesh : public Qtr3dGeometryBuffer
{
    Q_OBJECT
public:
    Qtr3dTexturedMesh(Qtr3dTextureFactory *textures, Qtr3d::MeshType meshType = Qtr3d::Triangle);
    Qtr3dTexturedMesh(Qtr3dTextureFactory *textures, const QString &textureName, Qtr3d::MeshType meshType = Qtr3d::Triangle);

    Qtr3d::MeshType meshType() const;
    void startMesh(Qtr3d::MeshType meshType = Qtr3d::Triangle, const QString &textureName = "");

    void addVertex(const QVector3D &vertex, Qtr3dScalar u, Qtr3dScalar v, const QVector3D &n = QVector3D());
    void addQuad(const QVector3D &p1, const QVector3D &p2, const QVector3D &p3, const QVector3D &p4, const QVector3D &n = QVector3D());
    void addIndex(int faceIndex);

    void endMesh();
    void reset();
    void destroy();
    void calculateNormals();

    // Shader Metadata
    inline QSize  texSize() const         { return mTexture ? QSize(mTexture->width(), mTexture->height()) : QSize(); }
    inline float  texRatio() const        { return mTexture && mTexture->height() > 0 ? (mTexture->width()/(float)mTexture->height()) : 0.0f; }
    inline GLuint vertexBufferId()  const { return mVertexBufferId;   }
    inline int    vertexCount() const     { return mIndexes.isEmpty() ? mVertices.count() : mIndexes.count();}
    inline GLuint elementBufferId() const { return mElementBufferId;  }
    inline GLuint textureId() const       { return mTexture ? mTexture->textureId():0; }
    GLenum        bufferType() const;

private:
    void init(const QString &fname);
    void releaseGlIds();

    Qtr3d::MeshType      mType;
    Qtr3dTextureFactory *mTextures;
    Qtr3dTexture        *mTexture;

    GLuint              mVertexBufferId;
    GLuint              mElementBufferId;

    QString                      mTextureName;
    QVector<Qtr3dTexturedVertex> mVertices;
    QVector<GLuint>              mIndexes;
};

typedef QList<Qtr3dTexturedMesh*>  Qtr3dTexturedMeshes;


#endif // QTR3DTEXTUREDMESH_H
