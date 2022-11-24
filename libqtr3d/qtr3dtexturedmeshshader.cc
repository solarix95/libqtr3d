#include "qtr3dtexturedmeshshader.h"


//-------------------------------------------------------------------------------------------------
Qtr3dTexturedMeshShader::Qtr3dTexturedMeshShader(const QString &eglFile)
    : Qtr3dShader(eglFile)
{
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedMeshShader::registerBuffer(Qtr3dTexturedMesh &buffer)
{
    mGeometryBuffers << &buffer;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedMeshShader::drawBuffers(const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, Qtr3dLightSource *light)
{
    for(auto *mesh: mGeometryBuffers) {
        const Qtr3dGeometryBufferStates &states = mesh->bufferStates();
        for(auto *state: states) {
            if (!state->enabled())
                continue;
            auto nextLightingTyp = state->lightingType();
            if (nextLightingTyp == Qtr3d::DefaultLighting)
                nextLightingTyp = Qtr3d::NoLighting;

            setProgram(nextLightingTyp);
            switch(nextLightingTyp) {
            case Qtr3d::NoLighting:
                drawBuffer_NoLight(*mesh, *state, perspectiveMatrix, worldMatrix);
                break;
            case Qtr3d::FlatLighting:
                drawBuffer_FlatLight(*mesh, *state, perspectiveMatrix, worldMatrix, light);
                break;
            case Qtr3d::PhongLighting:
                drawBuffer_PhongLight(*mesh, *state, perspectiveMatrix, worldMatrix, light);
                break;
            default:break;
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedMeshShader::onProgramChange()
{
    mVertexPosition   = currentProgram()->attributeLocation("vertex" );
    mVertexNormal     = currentProgram()->attributeLocation("vnormal" );
    mVertexTexcoords  = currentProgram()->attributeLocation("vtexcoords" );

    mModelviewMatrix  = currentProgram()->uniformLocation("modelview" );
    mProjectionMatrix = currentProgram()->uniformLocation("projection" );
    mLightPos         = currentProgram()->uniformLocation("lightpos" );
    mLightAmbient     = currentProgram()->uniformLocation("lightambientk" );
    mLightDiffuse     = currentProgram()->uniformLocation("lightdiffusek" );

    mDefaultTexture   = currentProgram()->uniformLocation("texture" );
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedMeshShader::drawBuffer_NoLight(const Qtr3dTexturedMesh &mesh, const Qtr3dGeometryBufferState &state, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix)
{
    QMatrix4x4 modelWorldMatrix = worldMatrix * state.modelView();

    currentProgram()->setUniformValue(mModelviewMatrix,modelWorldMatrix);
    currentProgram()->setUniformValue(mProjectionMatrix,perspectiveMatrix);

    drawMesh(mesh);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedMeshShader::drawBuffer_FlatLight(const Qtr3dTexturedMesh &mesh, const Qtr3dGeometryBufferState &state, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, Qtr3dLightSource *light)
{
    QMatrix4x4 modelWorldMatrix = worldMatrix * state.modelView();
    QVector3D lightPos = worldMatrix * light->pos();

    currentProgram()->setUniformValue(mModelviewMatrix,modelWorldMatrix);
    currentProgram()->setUniformValue(mProjectionMatrix,perspectiveMatrix);
    currentProgram()->setUniformValue(mLightPos,lightPos);
    currentProgram()->setUniformValue(mLightAmbient,light->strengthAmbient());
    currentProgram()->setUniformValue(mLightDiffuse,light->strengthDiffuse());
    drawMesh(mesh);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedMeshShader::drawBuffer_PhongLight(const Qtr3dTexturedMesh &mesh, const Qtr3dGeometryBufferState &state, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, Qtr3dLightSource *light)
{
    QMatrix4x4 modelWorldMatrix = worldMatrix * state.modelView();
    QVector3D lightPos = worldMatrix * light->pos();

    currentProgram()->setUniformValue(mModelviewMatrix,modelWorldMatrix);
    currentProgram()->setUniformValue(mProjectionMatrix,perspectiveMatrix);
    currentProgram()->setUniformValue(mLightPos,lightPos);
    currentProgram()->setUniformValue(mLightAmbient,light->strengthAmbient());
    currentProgram()->setUniformValue(mLightDiffuse,light->strengthDiffuse());

    currentProgram()->setUniformValue("material.ambient", light->strengthAmbient());
    currentProgram()->setUniformValue("material.diffuse", light->strengthDiffuse());
    currentProgram()->setUniformValue("material.specular",light->strengthSpecular());
    currentProgram()->setUniformValue("material.shininess",5.0f);

    currentProgram()->setUniformValue("lightambientcolor",light->ambientColor());
    drawMesh(mesh);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedMeshShader::drawMesh(const Qtr3dTexturedMesh &buffer)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    switch (buffer.faceOrientation()) {
    case Qtr3dGeometryBuffer::ClockWise:
        f->glFrontFace(GL_CW);
        break;
    case Qtr3dGeometryBuffer::CounterClockWise:
        f->glFrontFace(GL_CCW);
        break;
    default: break;
    }

    // Textures
    f->glActiveTexture( GL_TEXTURE0 );
    f->glBindTexture( GL_TEXTURE_2D, buffer.textureId() );
    f->glUniform1i( mDefaultTexture, 0 );

    // Vertices
    f->glBindBuffer( GL_ARRAY_BUFFER, buffer.vertexBufferId() );
    f->glVertexAttribPointer(
                mVertexPosition,
                4,
                GL_FLOAT,
                GL_FALSE,
                sizeof(Qtr3dTexturedVertex),
                (void*)0
                );
    f->glEnableVertexAttribArray( mVertexPosition );

    // Normals
    if (mVertexNormal >= 0) {
        f->glVertexAttribPointer(
                    mVertexNormal,
                    3,
                    GL_FLOAT,
                    GL_FALSE,
                    sizeof(Qtr3dTexturedVertex),
                    (void*)(sizeof(GLfloat) * 4)
                    );
        f->glEnableVertexAttribArray( mVertexNormal );
    }

    // Texture coordinates
    f->glVertexAttribPointer(
                mVertexTexcoords,
                2,
                GL_FLOAT,
                GL_FALSE,
                sizeof(Qtr3dTexturedVertex), // sizeof(GLfloat) * 9
                (void*)(sizeof(GLfloat) * 7)
                );
    f->glEnableVertexAttribArray( mVertexTexcoords );


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

    f->glDisableVertexAttribArray( mVertexTexcoords );
}


