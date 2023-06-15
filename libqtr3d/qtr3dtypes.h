#ifndef QTR3DTYPES_H
#define QTR3DTYPES_H

#include <QVector3D>
#include <QVector4D>
#include <QOpenGLTexture>
#include <QColor>
#include <GL/gl.h>

typedef GLfloat        Qtr3dScalar;
typedef QOpenGLTexture Qtr3dTexture;

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

struct MaterialLayer {
    QVector3D     strength;   // lighting strength
    QColor        mcolor;     // material color: has no effect if the vertex brings its own color or texture
    Qtr3dTexture *texture;
    quint32       textureId;

    MaterialLayer(float k, QColor c) : strength(k,k,k), mcolor(c) {};

    inline void         setStrength(float s) { strength = QVector3D(s,s,s); }
    inline QVector3D    mcolorf()  const     { return QVector3D(mcolor.redF(), mcolor.greenF(), mcolor.blueF()); }
    inline QVector4D    mcolorf4() const     { return QVector4D(mcolor.redF(), mcolor.greenF(), mcolor.blueF(), mcolor.alphaF()); }
};

class Material {
public:
    Material(float a = 0.0, float d=0.6, float s= 0.3, float shine = 5.0f)
        : mBaseColor(Qt::black)
        , mAmbient(a,Qt::white)
        , mDiffuse(d,Qt::white)
        , mSpecular(s,Qt::white)
        , mShininess(shine)
    {}

    inline QColor         baseColor() const { return mBaseColor; }
    inline MaterialLayer &ambient()  { return mAmbient;   }
    inline MaterialLayer &diffuse()  { return mDiffuse;   }
    inline MaterialLayer &specular() { return mSpecular;  }
    inline const MaterialLayer &ambient() const  { return mAmbient;   }
    inline const MaterialLayer &diffuse() const  { return mDiffuse;   }
    inline const MaterialLayer &specular() const { return mSpecular;  }
    inline float         shininess() const { return mShininess; }

    inline void setBaseColor(const QColor &c) { mBaseColor = c; }
    inline void setShininess(float s)         { mShininess = s; }

private:
    QColor        mBaseColor;
    MaterialLayer mAmbient;    // No lighting or no light
    MaterialLayer mDiffuse;    // Flat/Phong Lighting
    MaterialLayer mSpecular;   // Phong only
    float         mShininess;  // Phong only

   //  void setup(float ka, float kd, float ks, float s);
};

}

typedef struct Qtr3dVector_t
{
    Qtr3dScalar x,y,z;
    Qtr3dVector_t() : x(0.f), y(0.f), z(0.f) {}
    Qtr3dVector_t(float f1, float f2, float f3) : x(f1), y(f2), z(f3) {}
    Qtr3dVector_t(const QVector3D &v) : x(v.x()), y(v.y()), z(v.z()) {}
    inline QVector3D toQVector() const { return QVector3D(x,y,z); }
    inline bool      isNull() const    { return qFuzzyIsNull(x + y + z);}
    inline void      normalize()       { QVector3D vnorm = QVector3D(x,y,z).normalized(); x = vnorm.x(); y = vnorm.y(); z=vnorm.z(); }
    inline Qtr3dScalar  operator[](int i) const { if (i == 0) return x; if (i == 1) return y; if (i==2) return z; Q_ASSERT(0 && "invalid index"); return 0; }
    inline Qtr3dScalar &operator[](int i)       { if (i == 0) return x; if (i == 1) return y; if (i==2) return z; Q_ASSERT(0 && "invalid index"); return x; }
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

// Usage: Qtr3dMesh
typedef struct Qtr3dVertex_t {
    Qtr3dVector p;      // Point
    Qtr3dScalar w;      // "weight", add extra dimension to point for matrix multiplication
    Qtr3dVector n;      // Normal
    Qtr3dVector bi;     // boneIndices, "float" because Qt has no "glVertexAttribIPointer"!!!
    Qtr3dVector bw;     // boneWeights;

    inline Qtr3dVertex_t operator=(const Qtr3dVertex_t &other) {
        p = other.p;
        w = other.w;
        n = other.n;

        bi = other.bi;
        bw = other.bw;

        return other;
    }

    Qtr3dVertex_t() {
        p = {0.0f, 0.0f, 0.0f};
        w = 1.0f;
        n = {0.0f, 0.0f, 0.0f};

        bi = {0.0f, 0.0f, 0.0f};
        bw = {0.0f, 0.0f, 0.0f};
    }

    Qtr3dVertex_t(const QVector3D &v) {
        p = { v.x(), v.y(), v.z() };
        w = 1.0f;
        n = {0.0f, 0.0f, 0.0f};
        bi = {0.0f, 0.0f, 0.0f};
        bw = {0.0f, 0.0f, 0.0f};
    }

    Qtr3dVertex_t(const Qtr3dVertex_t &v) {
        p = v.p;
        w = v.w;
        n = v.n;

        bi = {0.0f, 0.0f, 0.0f};
        bw = {0.0f, 0.0f, 0.0f};
    }

    Qtr3dVertex_t(const QVector3D &v, const QVector3D &nv) {
        p = { v.x(), v.y(), v.z() };
        w = 1.0f;
        n = { nv.x(), nv.y(), nv.z() };
        bi = {0.0f, 0.0f, 0.0f};
        bw = {0.0f, 0.0f, 0.0f};
    }

    Qtr3dVertex_t(const Qtr3dVector &v, const Qtr3dVector &nv) {
        p = v;
        w = 1.0f;
        n = nv;
        bi = {0.0f, 0.0f, 0.0f};
        bw = {0.0f, 0.0f, 0.0f};
    }

    Qtr3dVertex_t(const Qtr3dVector &v, const Qtr3dVector &nv, const Qtr3dVector &bindexes, const Qtr3dVector &bweights) {
        p = v;
        w = 1.0f;
        n = nv;
        bi = bindexes;
        bw = bweights;
    }
} Qtr3dVertex;

// Usage: Qtr3dVertexMesh
typedef struct Qtr3dColoredVertex_t {
    Qtr3dVector p;   // Point
    Qtr3dVector n;   // Normal-Vector
    Qtr3dVector c;   // Color (x -> red, y -> green, z -> blue)
    float       a;   // Alpha
    Qtr3dVector bi;     // boneIndices, "float" because Qt has no "glVertexAttribIPointer"!!!
    Qtr3dVector bw;     // boneWeights;

    Qtr3dColoredVertex_t() {
        p = {0.0f, 0.0f, 0.0f};
        n = {0.0f, 0.0f, 0.0f};
        c = {0.0f, 0.0f, 0.0f};
        a = 1.0f;

        bi = {0.0f, 0.0f, 0.0f};
        bw = {0.0f, 0.0f, 0.0f};
    }
    Qtr3dColoredVertex_t(const QVector3D &v, const QColor &col) {
        p = { v.x(), v.y(), v.z() };
        n = {0.0f, 0.0f, 0.0f};
        c = { (float)col.redF(), (float)col.greenF(), (float)col.blueF() };
        a = col.alphaF();

        bi = {0.0f, 0.0f, 0.0f};
        bw = {0.0f, 0.0f, 0.0f};
    }
    Qtr3dColoredVertex_t(const QVector3D &v, const QVector3D &norm, const QColor &col) {
        p = { v.x(), v.y(), v.z() };
        n = { norm.x(), norm.y(), norm.z() };
        c = { (float)col.redF(), (float)col.greenF(), (float)col.blueF() };
        a = col.alphaF();

        bi = {0.0f, 0.0f, 0.0f};
        bw = {0.0f, 0.0f, 0.0f};
    }

    Qtr3dVertex toVertex() const { return Qtr3dVertex(p,n, bi, bw); }

} Qtr3dColoredVertex;

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

class Qtr3dEnvironment {
public:
    Qtr3dEnvironment() : mClearColor(Qt::black), mFogDistance(-1) {}
    inline QColor    clearColor()  const  { return mClearColor;  }
    inline QVector4D clearColor4f() const { return QVector4D(mClearColor.redF(),mClearColor.greenF(), mClearColor.blueF(), 1.0); }

    inline float  fogDistance() const { return mFogDistance; }

    inline void   setClearColor(QColor c) { mClearColor = c; }
    inline void   setFogDistance(float f) { mFogDistance= f; }

private:
    QColor mClearColor;
    float  mFogDistance;
};

#endif // QTR3DTYPES_H
