#ifndef QTR3DTEXTUREDSHADER_H
#define QTR3DTEXTUREDSHADER_H

#include "qtr3dshader.h"

class Qtr3dTexturedShader : public Qtr3dShader
{
public:
    Qtr3dTexturedShader(QObject *parent);
    virtual ~Qtr3dTexturedShader();

protected:
    virtual void onProgramChange();

private:
    virtual void drawBuffer_NoLight(const Qtr3dMesh    &mesh, const QMatrix4x4 &modelView, const QVector<QMatrix4x4> &meshSkeleton, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix);
    virtual void drawBuffer_FlatLight(const Qtr3dMesh  &mesh, const QMatrix4x4 &modelView, const QVector<QMatrix4x4> &meshSkeleton, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, const Qtr3dLightSource &light);
    virtual void drawBuffer_PhongLight(const Qtr3dMesh &mesh, const QMatrix4x4 &modelView, const QVector<QMatrix4x4> &meshSkeleton, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, const Qtr3dLightSource &light);

    //void drawBuffer_NoLight(const Qtr3dTexturedMesh    &mesh, const Qtr3dGeometryBufferState &state, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix);
    //void drawBuffer_FlatLight(const Qtr3dTexturedMesh  &mesh, const Qtr3dGeometryBufferState &state, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, const Qtr3dLightSource &light);
    // void drawBuffer_PhongLight(const Qtr3dTexturedMesh &mesh, const Qtr3dGeometryBufferState &state, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, const Qtr3dLightSource &light);

    void drawMesh(const Qtr3dMesh &buffer);

    // shader attributes
    int    mVertexPosition;
    int    mVertexNormal;
    int    mVertexTexcoords;
    int    mVertexBoneIndices;
    int    mVertexBoneWeights;

    int    mModelviewMatrix;
    int    mProjectionMatrix;
    int    mTexture;

    int    mLightPos;
    int    mLightAmbient;
    int    mLightDiffuse;
};

#endif // QTR3DTEXTUREDMESHSHADER_H
