#ifndef QTR3DTYPES_H
#define QTR3DTYPES_H

#include <QVector3D>
#include <QOpenGLTexture>
#include <QColor>
#include <GL/gl.h>

typedef GLfloat        Qtr3dScalar;
typedef QOpenGLTexture Qtr3dTexture;

typedef struct Qtr3dVector_t
{
    Qtr3dScalar x,y,z;
    Qtr3dVector_t() : x(0.f), y(0.f), z(0.f) {}
    Qtr3dVector_t(float f1, float f2, float f3) : x(f1), y(f2), z(f3) {}
    Qtr3dVector_t(const QVector3D &v) : x(v.x()), y(v.y()), z(v.z()) {}
    inline QVector3D toQVector() const { return QVector3D(x,y,z); }
    inline bool      isNull() const    { return qFuzzyIsNull(x + y + z);}
} Qtr3dVector;

inline bool operator==(const Qtr3dVector &v1, const QVector3D &v2)
{
    return v1.x == v2.x() && v1.y == v2.y() && v1.z == v2.z();
}

// Usage: Qtr3dTexturedQuad/Qtr3dTexturedQuadShader
typedef struct Qtr3dTexturedVertex_t {
    /*
      4x Float,
      3x Float
      2x Float
    */
    Qtr3dVector p;    // position
    Qtr3dScalar w;    // "weight", add extra dimension to point for matrix multiplication
    Qtr3dVector n;    // normal vector
    Qtr3dScalar t1;   // texture-coordinate [2]
    Qtr3dScalar t2;   // texture-coordinate [2]

    Qtr3dTexturedVertex_t() {
        p  = { 0.f, 0.f, 0.f };
        w  = 1.0f;
        n  = { 0.f, 0.f, 0.f };
        t1 = 0.f;
        t2 = 0.f;
    }

    Qtr3dTexturedVertex_t(const QVector3D &v,const QVector3D &norm, Qtr3dScalar tex1, Qtr3dScalar tex2 ) {
        p  = { v.x(), v.y(), v.z() };
        w  = 1.0f;
        n  = { norm.x(), norm.y(), norm.z() };
        t1 = tex1;
        t2 = tex2;
    }

    Qtr3dTexturedVertex_t(const QVector3D &v, const QVector3D &norm) {
        p = { v.x(), v.y(), v.z() };
        w = 1.0f;
        n = { norm.x(), norm.y(), norm.z() };
        t1 = 0.f;
        t2 = 0.f;
    }

} Qtr3dTexturedVertex;

// Usage: Qtr3dVertexMesh
typedef struct Qtr3dColoredVertex_t {
    Qtr3dVector p;   // Point
    Qtr3dScalar w;   // "weight", add extra dimension to point for matrix multiplication
    Qtr3dVector n;   // Normal-Vector
    Qtr3dVector c;   // Color (x -> red, y -> green, z -> blue)
    Qtr3dColoredVertex_t() {
        p = {0.0f, 0.0f, 0.0f};
        w = 1.0f;
        n = {0.0f, 0.0f, 0.0f};
        c = {0.0f, 0.0f, 0.0f};
    }
    Qtr3dColoredVertex_t(const QVector3D &v, const QColor &col) {
        p = { v.x(), v.y(), v.z() };
        w = 1.0f;
        n = {0.0f, 0.0f, 0.0f};
        c = { (float)col.redF(), (float)col.greenF(), (float)col.blueF() };
    }
    Qtr3dColoredVertex_t(const QVector3D &v, const QVector3D &norm, const QColor &col) {
        p = { v.x(), v.y(), v.z() };
        w = 1.0f;
        n = { norm.x(), norm.y(), norm.z() };
        c = { (float)col.redF(), (float)col.greenF(), (float)col.blueF() };
    }

} Qtr3dColoredVertex;

#endif // QTR3DTYPES_H
