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
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMeshShader::drawBuffer_NoLight(const Qtr3dVertexMesh &mesh, const Qtr3dGeometryBufferState &state, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix)
{
    // Normal view matrix - inverse transpose of modelview.
    QMatrix4x4 modelWorldMatrix = worldMatrix * state.modelView();

    currentProgram()->setUniformValue(mModelviewMatrix,modelWorldMatrix);
    currentProgram()->setUniformValue(mProjectionMatrix,perspectiveMatrix);

    drawMesh(mesh);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMeshShader::drawBuffer_FlatLight(const Qtr3dVertexMesh &mesh, const Qtr3dGeometryBufferState &state, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, Qtr3dLightSource *light)
{
    QMatrix4x4 modelWorldMatrix = worldMatrix * state.modelView();
    QVector3D lightPos = worldMatrix * light->pos();

    currentProgram()->setUniformValue(mModelviewMatrix,modelWorldMatrix);
    currentProgram()->setUniformValue(mProjectionMatrix,perspectiveMatrix);
    currentProgram()->setUniformValue(mLightPos,lightPos);
    drawMesh(mesh);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMeshShader::drawBuffer_PhongLight(const Qtr3dVertexMesh &mesh, const Qtr3dGeometryBufferState &state, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, Qtr3dLightSource *light)
{
    // Normal view matrix - inverse transpose of modelview.
    QMatrix4x4 modelWorldMatrix = worldMatrix * state.modelView();
    QVector3D lightPos = worldMatrix  * light->pos();

    currentProgram()->setUniformValue(mModelviewMatrix,modelWorldMatrix);
    currentProgram()->setUniformValue(mProjectionMatrix,perspectiveMatrix);
    currentProgram()->setUniformValue(mLightPos,lightPos);

    QMatrix4x4 normalView       = modelWorldMatrix.inverted();
    currentProgram()->setUniformValue(mNormalviewMatrix,normalView);

    drawMesh(mesh);


#if 0
    // Get locations of attributes and uniforms used inside.
    mVertexPosition   = mShaderProgramLight->attributeLocation("vertex" );
    mVertexNormal     = mShaderProgramLight->attributeLocation("vnormal" );
    mVertexColor      = mShaderProgramLight->attributeLocation("vcolor" );

    mModelviewMatrix  = mShaderProgramLight->uniformLocation("modelview" );
    mNormalviewMatrix = mShaderProgramLight->uniformLocation("normalview" );
    mProjectionMatrix = mShaderProgramLight->uniformLocation("projection" );
    mLightPos         = mShaderProgramLight->uniformLocation("lightPos" );

    Q_ASSERT(mProjectionMatrix >= 0);
    mShaderProgramLight->setUniformValue(mProjectionMatrix,perspectiveMatrix.transposed());

    mShaderProgramLight->setUniformValue("worldview",worldMatrix.transposed());

    Q_ASSERT(mLightPos >= 0);
    mShaderProgramLight->setUniformValue(mLightPos,QVector3D(0,-1000000,0));

    foreach(Qtr3dVertexMesh *mesh, mGeometryBuffers) {
        const Qtr3dGeometryBufferStates &states = mesh->bufferStates();
        foreach(Qtr3dGeometryBufferState *state, states) {
            if (state->isFlat())
                continue;

            // Normal view matrix - inverse transpose of modelview.
            QMatrix4x4 modelWorldMatrix = worldMatrix * state->modelView();
            QMatrix4x4 normalView       = modelWorldMatrix.inverted();

            mShaderProgramLight->setUniformValue(mModelviewMatrix,modelWorldMatrix.transposed());
            mShaderProgramLight->setUniformValue(mNormalviewMatrix,normalView.transposed());
            drawMesh(*mesh,worldMatrix*state->modelView()); // TODO: to shader..
        }
    }
#endif
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
                3,
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
