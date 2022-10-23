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
void Qtr3dVertexMeshShader::drawFlatBuffers(const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix)
{
    // Get locations of attributes and uniforms used inside.
    vertexPosition   = mShaderProgramFlat->attributeLocation("vertex" );
    vertexNormal     = mShaderProgramFlat->attributeLocation("vnormal" );
    vertexColor      = mShaderProgramFlat->attributeLocation("vcolor" );

    modelviewMatrix  = mShaderProgramFlat->uniformLocation("modelview" );
    normalviewMatrix = mShaderProgramFlat->uniformLocation("normalview" );
    projectionMatrix = mShaderProgramFlat->uniformLocation("projection" );

    mShaderProgramFlat->setUniformValue(projectionMatrix,perspectiveMatrix);

    foreach(Qtr3dVertexMesh *mesh, mGeometryBuffers) {
        const Qtr3dGeometryBufferStates &states = mesh->bufferStates();
        foreach(Qtr3dGeometryBufferState *state, states) {
            if (!state->isFlat())
                continue;
            drawMesh(*mesh,worldMatrix*state->modelView()); // TODO: to shader..
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMeshShader::drawLightBuffers(const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix)
{
    // Get locations of attributes and uniforms used inside.
    vertexPosition   = mShaderProgramLight->attributeLocation("vertex" );
    vertexNormal     = mShaderProgramLight->attributeLocation("vnormal" );
    vertexColor      = mShaderProgramLight->attributeLocation("vcolor" );

    modelviewMatrix  = mShaderProgramLight->uniformLocation("modelview" );
    normalviewMatrix = mShaderProgramLight->uniformLocation("normalview" );
    projectionMatrix = mShaderProgramLight->uniformLocation("projection" );
    lightPos         = mShaderProgramLight->uniformLocation("lightPos" );


    mShaderProgramLight->setUniformValue(projectionMatrix,perspectiveMatrix);
    mShaderProgramLight->setUniformValue(lightPos,QVector3D(10000000,10000000,10000000));

    foreach(Qtr3dVertexMesh *mesh, mGeometryBuffers) {
        const Qtr3dGeometryBufferStates &states = mesh->bufferStates();
        foreach(Qtr3dGeometryBufferState *state, states) {
            if (state->isFlat())
                continue;
            drawMesh(*mesh,worldMatrix*state->modelView()); // TODO: to shader..
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Qtr3dVertexMeshShader::drawMesh(const Qtr3dVertexMesh &buffer, const QMatrix4x4 &modelView)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    f->glFrontFace(buffer.vertexOrientation() == Qtr3dVertexMesh::CounterClockWise ? GL_CCW : GL_CW);
    // Vertices
    f->glBindBuffer( GL_ARRAY_BUFFER, buffer.vertexBufferId() );
    f->glVertexAttribPointer(
                vertexPosition,
                4,
                GL_FLOAT,
                GL_FALSE,
                sizeof(GLfloat) * 10,
                (void*)0
                );
    f->glEnableVertexAttribArray( vertexPosition );

    // Normals
    f->glVertexAttribPointer(
                vertexNormal,
                3,
                GL_FLOAT,
                GL_FALSE,
                sizeof(GLfloat) * 10,
                (void*)(sizeof(GLfloat) * 4)
                );
    f->glEnableVertexAttribArray( vertexNormal );

    // Colors
    f->glVertexAttribPointer(
                vertexColor,
                3,
                GL_FLOAT,
                GL_FALSE,
                sizeof(GLfloat) * 10,
                (void*)(sizeof(GLfloat) * 7)
                );
    f->glEnableVertexAttribArray( vertexColor );

    matrixAsUniform(f, modelviewMatrix, modelView );

    // Normal view matrix - inverse transpose of modelview.
    QMatrix4x4 normalView = modelView.inverted().transposed();
    matrixAsUniform(f, normalviewMatrix, normalView );

    // Send element buffer to GPU and draw.
    f->glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffer.elementBufferId() );
    f->glDrawElements(
                buffer.bufferType(),
                buffer.vertexCount(),
                GL_UNSIGNED_INT,
                (void*)0
                );

    // Clean up
    f->glDisableVertexAttribArray( vertexPosition );
    f->glDisableVertexAttribArray( vertexNormal );
    f->glDisableVertexAttribArray( vertexColor );
}
