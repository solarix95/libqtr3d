#ifndef QTR3DTEXTUREDMESHSHADER_H
#define QTR3DTEXTUREDMESHSHADER_H

#include "qtr3dtexturedmesh.h"
#include "qtr3dshader.h"

class Qtr3dTexturedMeshShader : public Qtr3dShader
{
public:
    Qtr3dTexturedMeshShader(const QString &eglFile);

    void registerBuffer(Qtr3dTexturedMesh &buffer);

protected:
    virtual void drawBuffers(const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, Qtr3dLightSource *light);
    virtual void onProgramChange();

private:

    void drawBuffer_NoLight(const Qtr3dTexturedMesh    &mesh, const Qtr3dGeometryBufferState &state, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix);
    void drawBuffer_FlatLight(const Qtr3dTexturedMesh  &mesh, const Qtr3dGeometryBufferState &state, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, Qtr3dLightSource *light);
    void drawBuffer_PhongLight(const Qtr3dTexturedMesh &mesh, const Qtr3dGeometryBufferState &state, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, Qtr3dLightSource *light);
    void drawMesh(const Qtr3dTexturedMesh &buffer);

    // These are variables passed into shaders
    int    mVertexPosition;
    int    mVertexNormal;
    int    mVertexTexcoords;

    int    mModelviewMatrix;
    int    mNormalviewMatrix;
    int    mProjectionMatrix;
    int    mLightPos;
    int    mLightAmbient;
    int    mLightDiffuse;

    int    mDefaultTexture;

    Qtr3dTexturedMeshes     mGeometryBuffers;
};

#endif // QTR3DTEXTUREDMESHSHADER_H
