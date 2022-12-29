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
    virtual void onProgramChange();

    void drawBuffer_NoLight(const Qtr3dMesh    &mesh, const QMatrix4x4 &modelView, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix);
    void drawBuffer_FlatLight(const Qtr3dMesh  &mesh, const QMatrix4x4 &modelView, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, const Qtr3dLightSource &light);
    void drawBuffer_PhongLight(const Qtr3dMesh &mesh, const QMatrix4x4 &modelView, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, const Qtr3dLightSource &light);

private:
    void drawMesh(const Qtr3dMesh &buffer);

    // Shader Attributes
    int mVertexPosition;
    int mVertexNormal;
    int mVertexColor;

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
