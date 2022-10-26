#ifndef QTR3DCOLOREDELEMENTS_H
#define QTR3DCOLOREDELEMENTS_H

#include <QVector>
#include <QObject>
#include "qtr3dgeometrybuffer.h"
#include "qtr3dtypes.h"

class Qtr3dVertexMesh : public Qtr3dGeometryBuffer
{
public:
    enum VertexOrientation {
        CounterClockWise,
        ClockWise
    };

    Qtr3dVertexMesh(Type meshType = Unknown);
    virtual ~Qtr3dVertexMesh();

    Type              meshType() const;
    VertexOrientation vertexOrientation() const;

    void reset();
    Qtr3dVertexMesh *startMesh(Type meshType, VertexOrientation orientation = CounterClockWise);
    void endMesh();

    void setDefaultColor(const QColor &c);
    void setVertexOrientation(VertexOrientation orientation);
    void addVertex(const QVector3D &v); // .. and use "defaultColor"
    void addVertex(const QVector3D &v, const QColor &c);
    void addVertex(const QVector3D &v, const QVector3D &n);
    void addVertex(const QVector3D &v, const QVector3D &n, const QColor &c);
    void addVertex(const Qtr3dColoredVertex &v);

    void addNormal(const QVector3D &n);
    void addIndex(int vi,int ti = -1, int ni = -1);

    // Metadata for generic viewers:
    virtual QVector3D minValues() const; // lowest xyz
    virtual QVector3D maxValues() const; // max xyz

    // Shader Interface
    inline GLuint vertexBufferId() const  { return mVertexBufferId;  }
    inline GLuint elementBufferId() const { return mElementBufferId; }
    inline int    vertexCount() const     { return mIndexes.isEmpty() ? mVertexes.count() : mIndexes.count();}
    GLenum        bufferType() const;

private:
    void analyze(const QVector3D &v);
    void calculateNormal(int vertexIndex);

    GLuint mVertexBufferId;
    GLuint mElementBufferId;

    QColor                      mDefaultColor;
    Type                        mMeshType;
    VertexOrientation           mVertexOrientation;
    QVector<Qtr3dColoredVertex> mVertexes;
    QVector<GLuint>             mIndexes;
    QVector3D                   mMin;
    QVector3D                   mMax;

    QList<QVector3D>            mNormals;
};


typedef QList<Qtr3dVertexMesh*> Qtr3dVertexMeshes;

#endif // QTR3DCOLOREDELEMENTS_H
