#include "qtr3dtexturedquadshader.h"

//-------------------------------------------------------------------------------------------------
Qtr3dTexturedQuadShader::Qtr3dTexturedQuadShader(const QString &eglFile)
    : Qtr3dShader(eglFile)
{
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedQuadShader::registerBuffer(Qtr3dTexturedQuad &buffer)
{
    mGeometryBuffers << &buffer;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedQuadShader::drawFlatBuffers(const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix)
{
    // Get locations of attributes and uniforms used inside.
    mVertexPosition   = mShaderProgramFlat->attributeLocation("vertex" );
    mVertexNormal     = mShaderProgramFlat->attributeLocation("vnormal" );
    mVertexTexcoords  = mShaderProgramFlat->attributeLocation("texcoords" );

    mModelviewMatrix  = mShaderProgramFlat->uniformLocation("modelview" );
    mNormalviewMatrix = mShaderProgramFlat->uniformLocation("normalview" );
    mProjectionMatrix = mShaderProgramFlat->uniformLocation("projection" );
    mWorldMatrix      = mShaderProgramFlat->uniformLocation("worldview" );

    mDefaultTexture   = mShaderProgramFlat->uniformLocation("texture" );

    mShaderProgramFlat->setUniformValue(mProjectionMatrix,perspectiveMatrix);
    mShaderProgramFlat->setUniformValue(mWorldMatrix,worldMatrix);

    foreach(Qtr3dTexturedQuad *quad, mGeometryBuffers) {
        const Qtr3dGeometryBufferStates &states = quad->bufferStates();
        foreach(Qtr3dGeometryBufferState *state, states) {
            if (!state->isFlat())
                continue;
            drawQuad(*quad,state->modelView());
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedQuadShader::drawLightBuffers(const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix)
{
    // Get locations of attributes and uniforms used inside.
    mVertexPosition   = mShaderProgramLight->attributeLocation("vertex" );
    mVertexNormal     = mShaderProgramLight->attributeLocation("vnormal" );
    mVertexTexcoords  = mShaderProgramLight->attributeLocation("texcoords" );

    mModelviewMatrix  = mShaderProgramLight->uniformLocation("modelview" );
    mNormalviewMatrix = mShaderProgramLight->uniformLocation("normalview" );
    mProjectionMatrix = mShaderProgramLight->uniformLocation("projection" );
    mWorldMatrix      = mShaderProgramFlat->uniformLocation("worldview" );

    mDefaultTexture   = mShaderProgramLight->uniformLocation("texture" );

    mShaderProgramLight->setUniformValue(mProjectionMatrix,perspectiveMatrix);
    mShaderProgramLight->setUniformValue(mWorldMatrix,worldMatrix);

    foreach(Qtr3dTexturedQuad *quad, mGeometryBuffers) {
        const Qtr3dGeometryBufferStates &states = quad->bufferStates();
        foreach(Qtr3dGeometryBufferState *state, states) {
            if (state->isFlat())
                continue;
            drawQuad(*quad,worldMatrix*state->modelView()); // TODO: to shader..
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedQuadShader::drawQuad(const Qtr3dTexturedQuad &buffer, const QMatrix4x4 &modelView)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    // Vertices
    f->glBindBuffer( GL_ARRAY_BUFFER, buffer.vertexBufferId() );
    f->glVertexAttribPointer(
                mVertexPosition,
                4,
                GL_FLOAT,
                GL_FALSE,
                sizeof(GLfloat) * 9,
                (void*)0
                );
    f->glEnableVertexAttribArray( mVertexPosition );

    // Normals
    f->glVertexAttribPointer(
                mVertexNormal,
                3,
                GL_FLOAT,
                GL_FALSE,
                sizeof(GLfloat) * 9,
                (void*)(sizeof(GLfloat) * 4)
                );
    f->glEnableVertexAttribArray( mVertexNormal );

    // Texture coordinates
    f->glVertexAttribPointer(
                mVertexTexcoords,
                2,
                GL_FLOAT,
                GL_FALSE,
                sizeof(GLfloat) * 9,
                (void*)(sizeof(GLfloat) * 7)
                );
    f->glEnableVertexAttribArray( mVertexTexcoords );

    QMatrix4x4 ratioView = modelView;
    ratioView.scale(buffer.texRatio(), 1, buffer.texRatio());
    // mShaderProgram->setUniformValue(modelviewMatrix,ratioView);
    matrixAsUniform( f, mModelviewMatrix, ratioView );

    // Normal view matrix - inverse transpose of modelview.
    QMatrix4x4 normalView = modelView.inverted().transposed();
    matrixAsUniform(f, mNormalviewMatrix, normalView );

    // Textures
    f->glActiveTexture( GL_TEXTURE0 );
    f->glBindTexture( GL_TEXTURE_2D, buffer.textureId() );
    f->glUniform1i( mDefaultTexture, 0 );

    // Send element buffer to GPU and draw.
    f->glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffer.elementBufferId() );
    f->glDrawElements(
                GL_QUADS,
                buffer.verticesCount(),
                GL_UNSIGNED_SHORT,
                (void*)0
                );

    // Clean up
    f->glDisableVertexAttribArray( mVertexPosition );
    f->glDisableVertexAttribArray( mVertexTexcoords );
}


