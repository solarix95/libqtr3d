#ifndef QTR3DPCSHADER_H
#define QTR3DPCSHADER_H

#include "qtr3dshader.h"
#include "../qtr3dpointcloud.h"

// lean PointCloud-Shader
class Qtr3dPcShader : public Qtr3dShader
{
public:
    Qtr3dPcShader(QObject *parent);
    virtual ~Qtr3dPcShader();

    virtual void onProgramChange() override;

    void render(const Qtr3dPointCloud &mesh, const QMatrix4x4 &modelView, const QVector<QMatrix4x4> &meshSkeleton, const Qtr3dCamera &camera, Qtr3d::LightingType lighting, const Qtr3dLightSource &light, const Qtr3dEnvironment &env);


    void drawBuffer_NoLight(const Qtr3dPointCloud    &mesh, const QMatrix4x4 &modelView, const QVector<QMatrix4x4> &meshSkeleton, const Qtr3dCamera &camera);
    void drawBuffer_FlatLight(const Qtr3dPointCloud  &mesh, const QMatrix4x4 &modelView, const QVector<QMatrix4x4> &meshSkeleton, const Qtr3dCamera &camera, const Qtr3dLightSource &light);
    void drawBuffer_PhongLight(const Qtr3dPointCloud &mesh, const QMatrix4x4 &modelView, const QVector<QMatrix4x4> &meshSkeleton, const Qtr3dCamera &camera, const Qtr3dLightSource &light);

protected:
    virtual void drawBuffer_NoLight(const Qtr3dMesh    &mesh, const QMatrix4x4 &modelView, const QVector<QMatrix4x4> &meshSkeleton, const Qtr3dCamera &camera) override;
    virtual void drawBuffer_FlatLight(const Qtr3dMesh  &mesh, const QMatrix4x4 &modelView, const QVector<QMatrix4x4> &meshSkeleton, const Qtr3dCamera &camera, const Qtr3dLightSource &light) override;
    virtual void drawBuffer_PhongLight(const Qtr3dMesh &mesh, const QMatrix4x4 &modelView, const QVector<QMatrix4x4> &meshSkeleton, const Qtr3dCamera &camera, const Qtr3dLightSource &light)override;
private:
    void drawMesh(const Qtr3dPointCloud &buffer);

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

#endif
