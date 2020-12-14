#ifndef QTR3DCOLOREDELEMENTS_H
#define QTR3DCOLOREDELEMENTS_H

#include <QVector>
#include <QObject>
#include "qtr3dgeometrybuffer.h"
#include "qtr3dtypes.h"

class Qtr3dVertexMesh : public Qtr3dGeometryBuffer
{
public:
    enum Type {
        Unknown,
        Dot,
        Line,
        Triangle,
        Quad
    };

    Qtr3dVertexMesh(Type meshType = Unknown);
    virtual ~Qtr3dVertexMesh();

    Type meshType() const;
    void reset();
    void startMesh(Type meshType);
    void endMesh();

    void setDefaultColor(const QColor &c);
    void addVertex(const QVector3D &v); // .. and use "defaultColor"
    void addVertex(const QVector3D &v, const QColor &c);
    void addVertex(const QVector3D &v, const QVector3D &n);
    void addVertex(const QVector3D &v, const QVector3D &n, const QColor &c);
    void addVertex(const Qtr3dColoredVertex &v);

    void addNormal(const QVector3D &n);
    void addIndex(int vi,int ti = -1, int ni = -1);

    void calculateNormals();

    // Shader Interface
    inline GLuint vertexBufferId() const  { return mVertexBufferId;  }
    inline GLuint elementBufferId() const { return mElementBufferId; }
    inline int    vertexCount() const     { return mIndexes.isEmpty() ? mVertexes.count() : mIndexes.count();}
    GLenum        bufferType() const;

private:
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
