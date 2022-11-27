#ifndef QTR3DCOLOREDELEMENTS_H
#define QTR3DCOLOREDELEMENTS_H

#include <QVector>
#include <QObject>
#include "qtr3dgeometrybuffer.h"
#include "qtr3dtypes.h"

class Qtr3dVertexMesh : public Qtr3dGeometryBuffer
{
public:
    Qtr3dVertexMesh(Type meshType = Unknown);
    virtual ~Qtr3dVertexMesh();

    void              setMeshType(Type t);
    Type              meshType() const;

    void reset();
    Qtr3dVertexMesh *startMesh(Type meshType, FaceOrientation orientation = DefaultOrientation);
    void endMesh();

    void setDefaultColor(const QColor &c);

    void addVertex(const QVector3D &v); // .. and use "defaultColor"
    void addVertex(const QVector3D &v, const QColor &c);
    void addVertex(const QVector3D &v, const QVector3D &n);
    void addVertex(const QVector3D &v, const QVector3D &n, const QColor &c);
    void addVertex(const Qtr3dColoredVertex &v);
    void addQuad(const QVector3D &p1, const QVector3D &p2, const QVector3D &p3, const QVector3D &p4, const QVector3D &n  = QVector3D());

    void addNormal(const QVector3D &n);
    void addIndex(int vi,int ti = -1, int ni = -1);

    // reader
    int                       vertexListCount() const;
    const Qtr3dColoredVertex &vertex(int i) const;

    // Shader Interface
    inline GLuint vertexBufferId() const  { return mVertexBufferId;  }
    inline GLuint elementBufferId() const { return mElementBufferId; }
    inline int    vertexCount() const     { return mIndexes.isEmpty() ? mVertexes.count() : mIndexes.count();}
    GLenum        bufferType() const;


private:
    void calculateNormal(int vertexIndex);

    GLuint mVertexBufferId;
    GLuint mElementBufferId;

    QColor                      mDefaultColor;
    Type                        mMeshType;

    QVector<Qtr3dColoredVertex> mVertexes;
    QVector<GLuint>             mIndexes;

    QList<QVector3D>            mNormals;
};


typedef QList<Qtr3dVertexMesh*> Qtr3dVertexMeshes;

#endif // QTR3DCOLOREDELEMENTS_H
