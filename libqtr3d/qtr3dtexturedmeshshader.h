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
    virtual void drawBuffers(const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix);
    virtual void onProgramChange();

    void drawBuffer_NoLight(const Qtr3dTexturedMesh    &mesh, const Qtr3dGeometryBufferState &state, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix);
    void drawBuffer_FlatLight(const Qtr3dTexturedMesh  &mesh, const Qtr3dGeometryBufferState &state, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix);
    void drawBuffer_PhongLight(const Qtr3dTexturedMesh &mesh, const Qtr3dGeometryBufferState &state, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix);


    virtual void drawFlatBuffers(const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &mWorldMatrix);
    virtual void drawLightBuffers(const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &mWorldMatrix);

private:
    void drawMesh(const Qtr3dTexturedMesh &buffer);

    // These are variables passed into shaders
    GLuint mVertexPosition;
    GLuint mVertexNormal;
    GLuint mVertexTexcoords;

    GLuint mModelviewMatrix;
    GLuint mNormalviewMatrix;
    GLuint mProjectionMatrix;
    GLuint mWorldMatrix;

    GLuint mDefaultTexture;

    Qtr3dTexturedMeshes     mGeometryBuffers;
};

#endif // QTR3DTEXTUREDMESHSHADER_H
