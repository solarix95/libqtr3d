#ifndef QTR3DTYPES_H
#define QTR3DTYPES_H

#include <QVector3D>
#include <QOpenGLTexture>
#include <QColor>
#include <GL/gl.h>

namespace Qtr3d
{

enum ShaderType {
    PlainShader,    // One Color per Mesh
    ColoredShader,  // One Color per Vertex
    TexturedShader  // UV-Texture per Mesh
};

enum MeshType {
    UnknownMesh,
    Dot,
    Line,
    Triangle,
    Quad
};

enum FaceOrientation {
    DefaultOrientation,
    ClockWise,
    CounterClockWise
};


enum LightingType {
    DefaultLighting,
    NoLighting,
    FlatLighting,
    PhongLighting
};

struct Material {
    QVector3D kAmbient;
    QVector3D kDiffuse;
    QVector3D kSpecular;
    float     shininess;

    Material(float a = 0.0, float d=0.3, float s= 0.3, float shine = 5.0f)
        : kAmbient(a,a,a)
        , kDiffuse(d,d,d)
        , kSpecular(s,s,s)
        , shininess(shine)
    {}
    void setup(float ka, float kd, float ks, float s);
};
}

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
    float       a;   // Alpha
    Qtr3dColoredVertex_t() {
        p = {0.0f, 0.0f, 0.0f};
        w = 1.0f;
        n = {0.0f, 0.0f, 0.0f};
        c = {0.0f, 0.0f, 0.0f};
        a = 1.0f;
    }
    Qtr3dColoredVertex_t(const QVector3D &v, const QColor &col) {
        p = { v.x(), v.y(), v.z() };
        w = 1.0f;
        n = {0.0f, 0.0f, 0.0f};
        c = { (float)col.redF(), (float)col.greenF(), (float)col.blueF() };
        a = col.alphaF();
    }
    Qtr3dColoredVertex_t(const QVector3D &v, const QVector3D &norm, const QColor &col) {
        p = { v.x(), v.y(), v.z() };
        w = 1.0f;
        n = { norm.x(), norm.y(), norm.z() };
        c = { (float)col.redF(), (float)col.greenF(), (float)col.blueF() };
        a = col.alphaF();
    }

} Qtr3dColoredVertex;

// Usage: Qtr3dMesh
typedef struct Qtr3dVertex_t {
    Qtr3dVector p;   // Point
    Qtr3dScalar w;   // "weight", add extra dimension to point for matrix multiplication
    Qtr3dVector n;   // Normal

    Qtr3dVertex_t() {
        p = {0.0f, 0.0f, 0.0f};
        w = 1.0f;
        n = {0.0f, 0.0f, 0.0f};
    }

    Qtr3dVertex_t(const QVector3D &v) {
        p = { v.x(), v.y(), v.z() };
        w = 1.0f;
        n = {0.0f, 0.0f, 0.0f};
    }

    Qtr3dVertex_t(const Qtr3dVertex_t &v) {
        p = v.p;
        w = v.w;
        n = v.n;
    }

    Qtr3dVertex_t(const QVector3D &v, const QVector3D &nv) {
        p = { v.x(), v.y(), v.z() };
        w = 1.0f;
        n = { nv.x(), nv.y(), nv.z() };
    }
} Qtr3dVertex;

// Usage: Qtr3dMesh
typedef struct Qtr3dColor_t {
    Qtr3dVector c;   // rgb float color
    Qtr3dScalar a;   // alpha float

    Qtr3dColor_t() {
        c = {0.0f, 0.0f, 0.0f};
        a = 1.0f;
    }

    Qtr3dColor_t(const QVector3D &ac, float aw = 1.0f) {
        c = { ac.x(), ac.y(), ac.z() };
        a = aw;
    }

    Qtr3dColor_t(const QColor &ac) {
        c = { float(ac.redF()), float(ac.greenF()), float(ac.blueF()) };
        a = ac.alphaF();
    }
} Qtr3dColor;

typedef struct Qtr3dTexCoord_t {
    Qtr3dScalar u;
    Qtr3dScalar v;

    Qtr3dTexCoord_t() : u(0.0), v(0.0) {}
    Qtr3dTexCoord_t(float au, float av) : u(au), v(av) {}
    Qtr3dTexCoord_t(const QPointF &p) : u(p.x()), v(p.y()) {}

} Qtr3dTexCoord;
#endif // QTR3DTYPES_H
