#ifndef QTR3DPLAINSHADER_H
#define QTR3DPLAINSHADER_H

#include "qtr3dshader.h"
#include "../qtr3dmesh.h"

class Qtr3dPlainShader : public Qtr3dShader
{
public:
    Qtr3dPlainShader(QObject *parent);
    virtual ~Qtr3dPlainShader();

protected:
    virtual void onProgramChange() override;

    virtual void drawBuffer_NoLight(const Qtr3dMesh    &mesh, const QMatrix4x4 &modelView, const QVector<QMatrix4x4> &meshSkeleton, const Qtr3dCamera &camera) override;
    virtual void drawBuffer_FlatLight(const Qtr3dMesh  &mesh, const QMatrix4x4 &modelView, const QVector<QMatrix4x4> &meshSkeleton, const Qtr3dCamera &camera, const Qtr3dLightSource &light) override;
    virtual void drawBuffer_PhongLight(const Qtr3dMesh &mesh, const QMatrix4x4 &modelView, const QVector<QMatrix4x4> &meshSkeleton, const Qtr3dCamera &camera, const Qtr3dLightSource &light) override;

private:
    void drawMesh(const Qtr3dMesh &buffer);

    // Shader global matrices
    int mProjectionMatrix;
    int mModelviewMatrix;
    int mModelColor;

    // Shader global light
    int mLightPos;
    int mLightColor;
    int mLightAmbient;
    int mLightDiffuse;

    // Shader Attributes
    int mVertexPosition;
    int mVertexNormal;
    int mVertexBoneIndices;
    int mVertexBoneWeights;

};

#endif
