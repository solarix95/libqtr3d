
#include <QOpenGLFunctions>
#include "qtr3dshader.h"
#include "qtr3dtexturedquad.h"

//-------------------------------------------------------------------------------------------------
// Simple helper to make a single buffer object.

/*
inline GLuint sMakeBO( QOpenGLFunctions *f, GLenum type, void* data, GLsizei size, int accessFlags ) {
    GLuint bo;
    f->glGenBuffers( 1, &bo );
    f->glBindBuffer( type, bo );
    f->glBufferData( type, size, data, accessFlags );
    return( bo );
}
*/

//-------------------------------------------------------------------------------------------------
Qtr3dShader::Qtr3dShader(const QString &eglFile)
{
    initShader(QString("%1_flat").arg(eglFile),mVertexShaderFlat, mFragmentShaderFlat, mShaderProgramFlat);
    initShader(QString("%1_phong").arg(eglFile),mVertexShaderLight, mFragmentShaderLight, mShaderProgramLight);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dShader::render(const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix)
{
    mShaderProgramFlat->bind();
    drawFlatBuffers(perspectiveMatrix, worldMatrix);
    mShaderProgramLight->bind();
    drawLightBuffers(perspectiveMatrix, worldMatrix);
}

//-------------------------------------------------------------------------------------------------
/*
GLuint Qtr3dShader::makeBO(void *data, GLsizei size, GLenum type)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    return sMakeBO(f,type, data, size, GL_STATIC_DRAW);

}
*/

// Simple helper to make a single buffer object.
GLuint Qtr3dShader::makeBO(void* data, GLsizei size, GLenum type, int accessFlags ) {
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    GLuint bo;
    f->glGenBuffers( 1, &bo );
    f->glBindBuffer( type, bo );
    f->glBufferData( type, size, data, accessFlags );

    return( bo );
}


//-------------------------------------------------------------------------------------------------
void Qtr3dShader::drawFlatBuffers(const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix)
{
}

//-------------------------------------------------------------------------------------------------
void Qtr3dShader::drawLightBuffers(const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix)
{
}

//-------------------------------------------------------------------------------------------------
void Qtr3dShader::initShader(const QString &name, QOpenGLShader *&vertex, QOpenGLShader *&fragment, QOpenGLShaderProgram *&program)
{
    vertex = new QOpenGLShader(QOpenGLShader::Vertex);
    vertex->compileSourceFile(QString(":/%1.vert").arg(name));
    Q_ASSERT(vertex->isCompiled());

    fragment = new QOpenGLShader(QOpenGLShader::Fragment);
    fragment->compileSourceFile(QString(":/%1.frag").arg(name));
    Q_ASSERT(fragment->isCompiled());

    program = new QOpenGLShaderProgram();
    program->addShader(vertex);
    program->addShader(fragment);

    bool done = program->link();
    Q_ASSERT_X(done,"Shader Linker", program->log().toUtf8().data());
}

