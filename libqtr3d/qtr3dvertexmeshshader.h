#ifndef QTR3DCOLOREDELEMENTSSHADER_H
#define QTR3DCOLOREDELEMENTSSHADER_H

#include "qtr3dshader.h"
#include "qtr3dvertexmesh.h"

class Qtr3dVertexMeshShader : public Qtr3dShader
{
public:
    Qtr3dVertexMeshShader(const QString &eglFile);

    void registerBuffer(Qtr3dVertexMesh &buffer);

protected:
    virtual void drawBuffers(const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, Qtr3dLightSource *light);
    virtual void onProgramChange();

    void drawBuffer_NoLight(const Qtr3dVertexMesh    &mesh, const Qtr3dGeometryBufferState &state, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix);
    void drawBuffer_FlatLight(const Qtr3dVertexMesh  &mesh, const Qtr3dGeometryBufferState &state, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, Qtr3dLightSource *light);
    void drawBuffer_PhongLight(const Qtr3dVertexMesh &mesh, const Qtr3dGeometryBufferState &state, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, Qtr3dLightSource *light);

private:

    void drawMesh(const Qtr3dVertexMesh &buffer);

    // Shader globals
    int mProjectionMatrix;
    int mModelviewMatrix; // world * model
    // int mWorldviewMatrix; // world * model
    int mNormalviewMatrix;

    // Shader Attributes
    int mVertexPosition;
    int mVertexNormal;
    int mVertexColor;
    int mLightPos;

    Qtr3dVertexMeshes     mGeometryBuffers;
};

#endif // QTR3DCOLOREDELEMENTSSHADER_H
