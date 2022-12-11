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
                nextLightingTyp = defaultLighting();

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
    currentProgram()->setUniformValue(mProjectionMatrix,perspectiveMatrix);

    const QList<QMatrix4x4> &transitions = mesh.modelTransitions();
    QMatrix4x4 modelWorldMatrix;
    if (transitions.isEmpty()) {
        modelWorldMatrix = worldMatrix * state.modelView();
        currentProgram()->setUniformValue(mModelviewMatrix,modelWorldMatrix);
        drawMesh(mesh);
    } else {
        for (const QMatrix4x4 &t: transitions) { // render all "blades" of a "fan"
            modelWorldMatrix = worldMatrix * state.modelView() * t;
            currentProgram()->setUniformValue(mModelviewMatrix,modelWorldMatrix);
            drawMesh(mesh);
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedMeshShader::drawBuffer_FlatLight(const Qtr3dTexturedMesh &mesh, const Qtr3dGeometryBufferState &state, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, Qtr3dLightSource *light)
{
    currentProgram()->setUniformValue(mProjectionMatrix,perspectiveMatrix);

    currentProgram()->setUniformValue("material.ambient", mesh.cMaterial().kAmbient);
    currentProgram()->setUniformValue("material.diffuse", mesh.cMaterial().kDiffuse);
    currentProgram()->setUniformValue("material.specular",mesh.cMaterial().kSpecular);
    currentProgram()->setUniformValue("material.shininess",5.0f);

    currentProgram()->setUniformValue("light.pos",     worldMatrix  * light->pos());
    currentProgram()->setUniformValue("light.ambient", light->strengthAmbient());
    currentProgram()->setUniformValue("light.color",   light->colorf());

    const QList<QMatrix4x4> &transitions = mesh.modelTransitions();
    QMatrix4x4 modelWorldMatrix;
    if (transitions.isEmpty()) {
        modelWorldMatrix = worldMatrix * state.modelView();
        currentProgram()->setUniformValue(mModelviewMatrix,modelWorldMatrix);
        drawMesh(mesh);
    } else {
        for (const QMatrix4x4 &t: transitions) { // render all "blades" of a "fan"
            modelWorldMatrix = worldMatrix * state.modelView() * t;
            currentProgram()->setUniformValue(mModelviewMatrix,modelWorldMatrix);
            drawMesh(mesh);
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedMeshShader::drawBuffer_PhongLight(const Qtr3dTexturedMesh &mesh, const Qtr3dGeometryBufferState &state, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, Qtr3dLightSource *light)
{
    currentProgram()->setUniformValue(mProjectionMatrix,perspectiveMatrix);

    currentProgram()->setUniformValue("material.ambient", mesh.cMaterial().kAmbient);
    currentProgram()->setUniformValue("material.diffuse", mesh.cMaterial().kDiffuse);
    currentProgram()->setUniformValue("material.specular",mesh.cMaterial().kSpecular);
    currentProgram()->setUniformValue("material.shininess",5.0f);

    currentProgram()->setUniformValue("light.pos",     worldMatrix  * light->pos());
    currentProgram()->setUniformValue("light.ambient", light->strengthAmbient());
    currentProgram()->setUniformValue("light.color",   light->colorf());

    const QList<QMatrix4x4> &transitions = mesh.modelTransitions();
    QMatrix4x4 modelWorldMatrix;
    if (transitions.isEmpty()) {
        modelWorldMatrix = worldMatrix * state.modelView();
        currentProgram()->setUniformValue(mModelviewMatrix,modelWorldMatrix);
        drawMesh(mesh);
    } else {
        for (const QMatrix4x4 &t: transitions) { // render all "blades" of a "fan"
            modelWorldMatrix = worldMatrix * state.modelView() * t;
            currentProgram()->setUniformValue(mModelviewMatrix,modelWorldMatrix);
            drawMesh(mesh);
        }
    }
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

    if (buffer.blending()) {
        f->glEnable(GL_BLEND);
        f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    } else {
        f->glDisable(GL_BLEND);
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


