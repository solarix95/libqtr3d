#ifndef QTR3DTEXTUREDQUAD_H
#define QTR3DTEXTUREDQUAD_H

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
class Qtr3dTexturedQuad : public Qtr3dGeometryBuffer
{
    Q_OBJECT
public:
    Qtr3dTexturedQuad(Qtr3dTextureFactory *textures);
    Qtr3dTexturedQuad(Qtr3dTextureFactory *textures, const QString &textureName);

    void startMesh(const QString &textureName = "");
    void addQuad(const QVector3D &p1,
                 const QVector3D &p2,
                 const QVector3D &p3,
                 const QVector3D &p4);

    void endMesh();
    void reset();
    void destroy();
    void calculateNormals();

    // Shader Metadata
    inline QSize  texSize() const         { return mTexture ? QSize(mTexture->width(), mTexture->height()) : QSize(); }
    inline float  texRatio() const        { return mTexture && mTexture->height() > 0 ? (mTexture->width()/(float)mTexture->height()) : 0.0f; }
    inline GLuint vertexBufferId()  const { return mVertexBufferId;   }
    inline GLuint verticesCount() const   { return mVertices.count(); }
    inline GLuint elementBufferId() const { return mElementBufferId;  }
    inline GLuint textureId() const       { return mTexture ? mTexture->textureId():0; }

private:
    void init(const QString &fname);
    void releaseGlIds();

    Qtr3dTextureFactory *mTextures;
    Qtr3dTexture        *mTexture;

    GLuint              mVertexBufferId;
    GLuint              mElementBufferId;

    QString                      mTextureName;
    QVector<Qtr3dTexturedVertex> mVertices;
};

typedef QList<Qtr3dTexturedQuad*>  Qtr3dTexturedQuads;

#endif // QTR3DTEXTUREDQUAD_H
