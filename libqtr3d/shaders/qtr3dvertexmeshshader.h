#ifndef QTR3DCOLOREDELEMENTSSHADER_H
#define QTR3DCOLOREDELEMENTSSHADER_H

#include "qtr3dshader.h"
#include "../qtr3dmesh.h"

class Qtr3dVertexMeshShader : public Qtr3dShader
{
public:
    Qtr3dVertexMeshShader(QObject *parent);
    virtual ~Qtr3dVertexMeshShader();

protected:
    virtual void onProgramChange() override;

    void drawBuffer_NoLight(const Qtr3dMesh    &mesh, const QMatrix4x4 &modelView, const QVector<QMatrix4x4> &meshSkeleton, const Qtr3dCamera &camera) override;
    void drawBuffer_FlatLight(const Qtr3dMesh  &mesh, const QMatrix4x4 &modelView, const QVector<QMatrix4x4> &meshSkeleton, const Qtr3dCamera &camera, const Qtr3dLightSource &light) override;
    void drawBuffer_PhongLight(const Qtr3dMesh &mesh, const QMatrix4x4 &modelView, const QVector<QMatrix4x4> &meshSkeleton, const Qtr3dCamera &camera, const Qtr3dLightSource &light) override;

private:
    void drawMesh(const Qtr3dMesh &buffer);

    // Shader Attributes
    int mVertexPosition;
    int mVertexNormal;
    int mVertexColor;
    int mVertexBoneIndices;
    int mVertexBoneWeights;

    // Shader global matrices
    int mProjectionMatrix;
    int mModelviewMatrix; // world * model

    // Shader global light
    int mLightPos;
    int mLightColor;
    int mLightAmbient;
    int mLightDiffuse;
};

#endif // QTR3DCOLOREDELEMENTSSHADER_H
