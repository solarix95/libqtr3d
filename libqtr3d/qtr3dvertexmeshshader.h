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
    virtual void drawFlatBuffers(const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix);
    virtual void drawLightBuffers(const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix);

private:
    void drawMesh(const Qtr3dVertexMesh &buffer, const QMatrix4x4 &modelView);

    // Shader globals
    GLuint projectionMatrix;
    GLuint modelviewMatrix;
    GLuint normalviewMatrix;

    // Shader Attributes
    GLuint vertexPosition;
    GLuint vertexNormal;
    GLuint vertexColor;
    GLuint lightPos;

    Qtr3dVertexMeshes     mGeometryBuffers;
};

#endif // QTR3DCOLOREDELEMENTSSHADER_H
