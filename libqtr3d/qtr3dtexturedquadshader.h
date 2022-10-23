#ifndef QTR3DTEXTUREDQUADSHADER_H
#define QTR3DTEXTUREDQUADSHADER_H

#include "qtr3dtexturedquad.h"
#include "qtr3dshader.h"

class Qtr3dTexturedQuadShader : public Qtr3dShader
{
public:
    Qtr3dTexturedQuadShader(const QString &eglFile);

    void registerBuffer(Qtr3dTexturedQuad &buffer);

protected:
    virtual void drawFlatBuffers(const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &mWorldMatrix);
    virtual void drawLightBuffers(const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &mWorldMatrix);

private:
    void drawQuad(const Qtr3dTexturedQuad &buffer, const QMatrix4x4 &modelView);

    // These are variables passed into shaders
    GLuint mVertexPosition;
    GLuint mVertexNormal;
    GLuint mVertexTexcoords;

    GLuint mModelviewMatrix;
    GLuint mNormalviewMatrix;
    GLuint mProjectionMatrix;
    GLuint mWorldMatrix;

    GLuint mDefaultTexture;

    Qtr3dTexturedQuads     mGeometryBuffers;
};

#endif // QTR3DTEXTUREDQUADSHADER_H
