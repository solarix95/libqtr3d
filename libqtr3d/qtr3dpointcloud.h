#ifndef QTR3DPOINTCLOUD_H
#define QTR3DPOINTCLOUD_H

#include <QVector>
#include <QObject>
#include <QList>
#include "qtr3dgeometry.h"
#include "qtr3dtypes.h"

class Qtr3dPointCloud : public Qtr3dGeometry
{
public:
    explicit Qtr3dPointCloud(Qtr3dAssets *parent);
    virtual ~Qtr3dPointCloud();

    void              reset();
    Qtr3dPointCloud  *startCloud();
    void              endCloud(bool doTrim = false);
    void              trim(); // free memory by throw away unused data structures

    void addVertex(const QVector3D &v); // .. and use "defaultColor"
    void addVertex(const QVector3D &v, const QColor &c);

    // reader
    int                 verticesCount() const;
    const Qtr3dPcVertex &vertex(int i) const;

    // Shader Interface
    inline GLuint vertexBufferId() const  { return mVerticesBufferId;  }
    inline GLuint elementBufferId() const { return mElementBufferId; }
    inline GLuint colorBufferId() const   { return mColorBufferId; }
    inline int    renderedVerticesCount() const   { return mVertexCount > 0 ? mVertexCount : mVertices.count(); }
    GLenum        bufferType() const;

private:
    GLuint mVerticesBufferId;
    GLuint mElementBufferId;
    GLuint mColorBufferId;
    int    mVertexCount;

    QVector<Qtr3dPcVertex>   mVertices;
    QVector<GLuint>          mIndexes;
    QVector<Qtr3dVector>     mColors;
};

typedef QList<Qtr3dPointCloud*> Qtr3dPointClouds;

#endif // QTR3DPOINTCLOUD_H
