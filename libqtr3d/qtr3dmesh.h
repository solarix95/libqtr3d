#ifndef QTR3DCOLOREDELEMENTS_H
#define QTR3DCOLOREDELEMENTS_H

#include <QVector>
#include <QObject>
#include <QList>
#include "qtr3dgeometry.h"
#include "qtr3dtypes.h"

class QOpenGLTexture;
class Qtr3dMesh : public Qtr3dGeometry
{
public:
    struct BoneWeight {
        int   vertexIndex;
        float weight;
        BoneWeight(int i, float w): vertexIndex(i), weight(w) {}
    };

    struct Bone {
        QString           name;
        QMatrix4x4        offset;
        QList<BoneWeight> weights;
    };

    Qtr3dMesh(Qtr3dAssets *parent);
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
    void addQuad(const QVector3D &p1, const QVector3D &p2, const QVector3D &p3, const QVector3D &p4, const QColor &c, const QVector3D &n  = QVector3D());

    // Mesh Building
    void addBone(const Bone &bone);
    void addNormal(const QVector3D &n);
    void updateBone(int vi, const Qtr3dVector &bi, const Qtr3dVector &bw);
    void addIndex(int vi, int ni = -1);

    void setTexture(const QImage &img);
    bool hasTexture() const;

    // reader
    int               verticesCount() const;
    const Qtr3dVertex &vertex(int i) const;
    const QList<Bone> &bones() const;

    // Shader Interface
    inline GLuint vertexBufferId() const  { return mVerticesBufferId;  }
    inline GLuint elementBufferId() const { return mElementBufferId; }
    inline int    renderedVerticesCount() const   { return mVertexCount > 0 ? mVertexCount : (mIndexes.isEmpty() ? mVertices.count() : mIndexes.count());}
    inline GLuint colorBufferId() const   { return mColorBufferId;  }
    inline GLuint texcoordBufferId() const{ return mTexcoorBufferId;  }
    inline GLuint textureId() const       { return mTexture ? mTexture->textureId():0; }
    GLenum        bufferType() const;
    QVector<QMatrix4x4> animatedSkeleton() const;

private:
    void initMeshSkeleton();
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
    QList<Bone>              mBones;

    QList<QVector3D>         mNormals;

    QVector<Qtr3dColor>      mColors;
    QVector<Qtr3dTexCoord>   mTextureCoords;

    QOpenGLTexture          *mTexture;
};

typedef QList<Qtr3dMesh*> Qtr3dMeshes;

#endif // QTR3DCOLOREDELEMENTS_H
