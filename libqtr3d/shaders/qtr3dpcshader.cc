#include "qtr3dpcshader.h"
#include "../qtr3dcamera.h"
#include "../qtr3dgeometrystate.h"
#include "../qtr3dlightsource.h"

//-------------------------------------------------------------------------------------------------
Qtr3dPcShader::Qtr3dPcShader(QObject *parent)
    : Qtr3dShader(parent, "pointcloud","pointcloud")
{
}

Qtr3dPcShader::~Qtr3dPcShader() = default;

//-------------------------------------------------------------------------------------------------
void Qtr3dPcShader::onProgramChange()
{
    // Get locations of attributes and uniforms used inside.
    mVertexPosition   = currentProgram()->attributeLocation("vertex" );
    mVertexColor      = currentProgram()->attributeLocation("vcolor" );

    mProjectionMatrix = currentProgram()->uniformLocation("projection" );
    mModelviewMatrix  = currentProgram()->uniformLocation("modelview" );

}

//-------------------------------------------------------------------------------------------------
void Qtr3dPcShader::render(const Qtr3dPointCloud &mesh, const QMatrix4x4 &modelView, const QVector<QMatrix4x4> &meshSkeleton, const Qtr3dCamera &camera, Qtr3d::LightingType lighting, const Qtr3dLightSource &light, const Qtr3dEnvironment &env)
{
    currentProgram()->bind();

    currentProgram()->setUniformValue("fog.color",     env.clearColor4f());
    currentProgram()->setUniformValue("fog.distance",  env.fogDistance());

    drawBuffer_NoLight(mesh, modelView, meshSkeleton, camera.projection() , camera.worldMatrix());
}

//-------------------------------------------------------------------------------------------------
void Qtr3dPcShader::drawBuffer_NoLight(const Qtr3dPointCloud &mesh, const QMatrix4x4 &modelView, const QVector<QMatrix4x4> &meshSkeleton, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix)
{
    currentProgram()->setUniformValue(mProjectionMatrix,perspectiveMatrix);
    currentProgram()->setUniformValue(mModelviewMatrix,worldMatrix * modelView);
    drawMesh(mesh);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dPcShader::drawMesh(const Qtr3dPointCloud &buffer)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glDisable(GL_BLEND);


    // Vertices
    f->glBindBuffer( GL_ARRAY_BUFFER, buffer.vertexBufferId() );
    f->glVertexAttribPointer(
                mVertexPosition,
                3,
                GL_FLOAT,
                GL_FALSE,
                sizeof(Qtr3dPcVertex),
                (void*)0
                );
    f->glEnableVertexAttribArray( mVertexPosition );

    // Colors
    f->glBindBuffer( GL_ARRAY_BUFFER, buffer.colorBufferId() );
    f->glVertexAttribPointer(
                mVertexColor,
                3,             // RGB + A
                GL_FLOAT,      // .. each "Float"
                GL_FALSE,
                sizeof(Qtr3dVector),
                (void*)0
                );
    f->glEnableVertexAttribArray( mVertexColor );

    // Send element buffer to GPU and draw.
    f->glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffer.elementBufferId() );
    f->glDrawElements(
                buffer.bufferType(),
                buffer.renderedVerticesCount(),
                GL_UNSIGNED_INT,
                (void*)0
                );

    // Clean up
    f->glDisableVertexAttribArray( mVertexPosition );
    f->glDisableVertexAttribArray( mVertexColor );
}
