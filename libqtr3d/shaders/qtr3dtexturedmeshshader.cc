
#include "qtr3dtexturedmeshshader.h"
#include "../qtr3dlightsource.h"
#include "../qtr3dmesh.h"

//-------------------------------------------------------------------------------------------------
Qtr3dTexturedShader::Qtr3dTexturedShader(QObject *parent)
    : Qtr3dShader(parent, "textured", "varying vec2 fragTexcoords; uniform sampler2D textureId;", "texture2D( textureId, fragTexcoords );")
    , mVertexPosition(-1)
    , mVertexNormal(-1)
    , mVertexTexcoords(-1)
    , mModelviewMatrix(-1)
    , mProjectionMatrix(-1)
{}
Qtr3dTexturedShader::~Qtr3dTexturedShader() = default;

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedShader::onProgramChange()
{
    mVertexPosition   = currentProgram()->attributeLocation("vertex");
    mVertexNormal     = currentProgram()->attributeLocation("vnormal");
    mVertexTexcoords  = currentProgram()->attributeLocation("vtexcoords");

    mModelviewMatrix  = currentProgram()->uniformLocation("modelview");
    mProjectionMatrix = currentProgram()->uniformLocation("projection");

    mTexture   = currentProgram()->uniformLocation("textureId" );
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedShader::drawBuffer_NoLight(const Qtr3dMesh &mesh, const QMatrix4x4 &modelView, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix)
{
    currentProgram()->setUniformValue(mProjectionMatrix,perspectiveMatrix);
    currentProgram()->setUniformValue(mModelviewMatrix,worldMatrix * modelView);
    drawMesh(mesh);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTexturedShader::drawBuffer_FlatLight(const Qtr3dMesh &mesh, const QMatrix4x4 &modelView, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, const Qtr3dLightSource &light)
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
void Qtr3dTexturedShader::drawBuffer_PhongLight(const Qtr3dMesh &mesh, const QMatrix4x4 &modelView, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, const Qtr3dLightSource &light)
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
void Qtr3dTexturedShader::drawMesh(const Qtr3dMesh &buffer)
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

    // Textures
    f->glActiveTexture( GL_TEXTURE0 );
    f->glBindTexture( GL_TEXTURE_2D, buffer.textureId() );
    f->glUniform1i( mTexture, 0 );

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

    // Normals
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

    // Texture coordinates
    f->glBindBuffer( GL_ARRAY_BUFFER, buffer.texcoordBufferId() );
    f->glVertexAttribPointer(
                mVertexTexcoords,
                2,
                GL_FLOAT,
                GL_FALSE,
                sizeof(Qtr3dTexCoord), // sizeof(GLfloat) * 9
                (void*)0
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


