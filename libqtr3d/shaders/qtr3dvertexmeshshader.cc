#include "qtr3dvertexmeshshader.h"
#include "../qtr3dgeometrystate.h"
#include "../qtr3dlightsource.h"

//-------------------------------------------------------------------------------------------------
Qtr3dVertexMeshShader::Qtr3dVertexMeshShader(QObject *parent)
    : Qtr3dShader(parent, "colored", "varying vec4 fragColor;", "fragColor;")
{
}

Qtr3dVertexMeshShader::~Qtr3dVertexMeshShader() = default;

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMeshShader::onProgramChange()
{
    // Get locations of attributes and uniforms used inside.
    mVertexPosition   = currentProgram()->attributeLocation("vertex" );
    mVertexNormal     = currentProgram()->attributeLocation("vnormal" );
    mVertexColor      = currentProgram()->attributeLocation("vcolor" );

    mProjectionMatrix = currentProgram()->uniformLocation("projection" );
    mModelviewMatrix  = currentProgram()->uniformLocation("modelview" );

    mLightPos         = currentProgram()->uniformLocation("lightpos" );
    mLightColor       = currentProgram()->uniformLocation("lightcolor" );
    mLightAmbient     = currentProgram()->uniformLocation("lightambientk" );
    mLightDiffuse     = currentProgram()->uniformLocation("lightdiffusek" );
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMeshShader::drawBuffer_NoLight(const Qtr3dMesh &mesh, const QMatrix4x4 &modelView, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix)
{
    currentProgram()->setUniformValue(mProjectionMatrix,perspectiveMatrix);
    currentProgram()->setUniformValue(mModelviewMatrix,worldMatrix * modelView);
    drawMesh(mesh);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMeshShader::drawBuffer_FlatLight(const Qtr3dMesh &mesh, const QMatrix4x4 &modelView, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, const Qtr3dLightSource &light)
{
    QVector3D lightPos = worldMatrix * light.pos();

    currentProgram()->setUniformValue(mProjectionMatrix,perspectiveMatrix);
    // currentProgram()->setUniformValue(mNormalviewMatrix,modelWorldMatrix.inverted());

    currentProgram()->setUniformValue(mLightPos,lightPos);

    currentProgram()->setUniformValue("material.ambient",mesh.material().ambient().strength);
    currentProgram()->setUniformValue("material.diffuse",mesh.material().diffuse().strength);

    currentProgram()->setUniformValue("light.pos",     worldMatrix  * light.pos());
    currentProgram()->setUniformValue("light.ambient", light.strengthAmbient());
    currentProgram()->setUniformValue("light.color",   light.colorf());

    currentProgram()->setUniformValue(mModelviewMatrix,worldMatrix * modelView);
    drawMesh(mesh);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMeshShader::drawBuffer_PhongLight(const Qtr3dMesh &mesh, const QMatrix4x4 &modelView, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, const Qtr3dLightSource &light)
{
    currentProgram()->setUniformValue(mProjectionMatrix,perspectiveMatrix);

    currentProgram()->setUniformValue("material.ambient", mesh.material().ambient().strength);
    currentProgram()->setUniformValue("material.diffuse", mesh.material().diffuse().strength);
    currentProgram()->setUniformValue("material.specular",mesh.material().specular().strength);
    currentProgram()->setUniformValue("material.shininess",mesh.material().shininess());

    currentProgram()->setUniformValue("light.pos",     worldMatrix  * light.pos());
    currentProgram()->setUniformValue("light.ambient", light.strengthAmbient());
    currentProgram()->setUniformValue("light.color",   light.colorf());

    currentProgram()->setUniformValue(mModelviewMatrix,worldMatrix * modelView);
    drawMesh(mesh);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMeshShader::drawMesh(const Qtr3dMesh &buffer)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    switch (buffer.faceOrientation()) {
    case Qtr3d::ClockWise:
        f->glFrontFace(GL_CW);
        break;
    case Qtr3d::CounterClockWise:
        f->glFrontFace(GL_CCW);
        break;
    default: break;
    }

    if (buffer.blending()) {
        f->glEnable(GL_BLEND);
        f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        f->glDisable(GL_BLEND);
    }

    // Vertices
    f->glBindBuffer( GL_ARRAY_BUFFER, buffer.vertexBufferId() );
    f->glVertexAttribPointer(
                mVertexPosition,
                4,
                GL_FLOAT,
                GL_FALSE,
                sizeof(Qtr3dVertex),
                (void*)0
                );
    f->glEnableVertexAttribArray( mVertexPosition );

    // Normals -> lighting only
    if (mVertexNormal >= 0) {
        f->glVertexAttribPointer(
                    mVertexNormal,
                    3,
                    GL_FLOAT,
                    GL_FALSE,
                    sizeof(Qtr3dVertex),
                    (void*)(sizeof(GLfloat) * 4)
                    );
        f->glEnableVertexAttribArray( mVertexNormal );
    }

    // Colors
    f->glBindBuffer( GL_ARRAY_BUFFER, buffer.colorBufferId() );
    f->glVertexAttribPointer(
                mVertexColor,
                4,             // RGB + A
                GL_FLOAT,      // .. each "Float"
                GL_FALSE,
                sizeof(Qtr3dColor),
                (void*)0
                );
    f->glEnableVertexAttribArray( mVertexColor );

    // Send element buffer to GPU and draw.
    f->glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffer.elementBufferId() );
    f->glDrawElements(
                buffer.bufferType(),
                buffer.vertexCount(),
                GL_UNSIGNED_INT,
                (void*)0
                );

    // Clean up
    f->glDisableVertexAttribArray( mVertexPosition );
    if (mVertexNormal >= 0)
        f->glDisableVertexAttribArray( mVertexNormal );
    f->glDisableVertexAttribArray( mVertexColor );
}
