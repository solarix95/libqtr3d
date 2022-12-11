#include "qtr3dvertexmeshshader.h"
#include "qtr3dgeometrybufferstate.h"

//-------------------------------------------------------------------------------------------------
Qtr3dVertexMeshShader::Qtr3dVertexMeshShader(const QString &eglFile)
    : Qtr3dShader(eglFile)
{
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMeshShader::registerBuffer(Qtr3dVertexMesh &buffer)
{
    mGeometryBuffers << &buffer;
    connect(&buffer, &Qtr3dVertexMesh::destroyed, this, [&](QObject *obj) {
        mGeometryBuffers.removeAll((Qtr3dVertexMesh*)obj);
    });
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMeshShader::drawBuffers(const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, Qtr3dLightSource *light)
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
void Qtr3dVertexMeshShader::onProgramChange()
{
    // Get locations of attributes and uniforms used inside.
    mVertexPosition   = currentProgram()->attributeLocation("vertex" );
    mVertexNormal     = currentProgram()->attributeLocation("vnormal" );
    mVertexColor      = currentProgram()->attributeLocation("vcolor" );

    mProjectionMatrix = currentProgram()->uniformLocation("projection" );
    mModelviewMatrix  = currentProgram()->uniformLocation("modelview" );
    mNormalviewMatrix = currentProgram()->uniformLocation("normalview" );

    mLightPos         = currentProgram()->uniformLocation("lightpos" );
    mLightColor       = currentProgram()->uniformLocation("lightcolor" );
    mLightAmbient     = currentProgram()->uniformLocation("lightambientk" );
    mLightDiffuse     = currentProgram()->uniformLocation("lightdiffusek" );
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMeshShader::drawBuffer_NoLight(const Qtr3dVertexMesh &mesh, const Qtr3dGeometryBufferState &state, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix)
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
void Qtr3dVertexMeshShader::drawBuffer_FlatLight(const Qtr3dVertexMesh &mesh, const Qtr3dGeometryBufferState &state, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, Qtr3dLightSource *light)
{
    QVector3D lightPos = worldMatrix * light->pos();

    currentProgram()->setUniformValue(mProjectionMatrix,perspectiveMatrix);
    // currentProgram()->setUniformValue(mNormalviewMatrix,modelWorldMatrix.inverted());

    currentProgram()->setUniformValue(mLightPos,lightPos);

    currentProgram()->setUniformValue("material.ambient",mesh.cMaterial().kAmbient);
    currentProgram()->setUniformValue("material.diffuse",mesh.cMaterial().kDiffuse);

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
void Qtr3dVertexMeshShader::drawBuffer_PhongLight(const Qtr3dVertexMesh &mesh, const Qtr3dGeometryBufferState &state, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, Qtr3dLightSource *light)
{
    currentProgram()->setUniformValue(mProjectionMatrix,perspectiveMatrix);

    currentProgram()->setUniformValue("material.ambient", mesh.cMaterial().kAmbient);
    currentProgram()->setUniformValue("material.diffuse", mesh.cMaterial().kDiffuse);
    currentProgram()->setUniformValue("material.specular",mesh.cMaterial().kSpecular);
    currentProgram()->setUniformValue("material.shininess",mesh.cMaterial().shininess);

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
void Qtr3dVertexMeshShader::drawMesh(const Qtr3dVertexMesh &buffer)
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

    // Vertices
    f->glBindBuffer( GL_ARRAY_BUFFER, buffer.vertexBufferId() );
    f->glVertexAttribPointer(
                mVertexPosition,
                4,
                GL_FLOAT,
                GL_FALSE,
                sizeof(Qtr3dColoredVertex),
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
                    sizeof(Qtr3dColoredVertex),
                    (void*)(sizeof(GLfloat) * 4)
                    );
        f->glEnableVertexAttribArray( mVertexNormal );
    }

    // Colors
    f->glVertexAttribPointer(
                mVertexColor,
                4,
                GL_FLOAT,
                GL_FALSE,
                sizeof(Qtr3dColoredVertex),
                (void*)(sizeof(GLfloat) * 7)
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
