
#include <QOpenGLFunctions>
#include "qtr3dshader.h"
#include "qtr3dtexturedquad.h"

//-------------------------------------------------------------------------------------------------
Qtr3dShader::Qtr3dShader(const QString &eglFile)
 : mDefaultLighting(Qtr3d::NoLighting)
 , mCurrentType(Qtr3d::DefaultLighting)
{
    initShader(Qtr3d::NoLighting,    eglFile);
    initShader(Qtr3d::FlatLighting,  eglFile);
    initShader(Qtr3d::PhongLighting, eglFile);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dShader::setDefaultLighting(Qtr3d::LightingType l)
{
    mDefaultLighting = l;
    Q_ASSERT(mDefaultLighting != Qtr3d::DefaultLighting);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dShader::render(const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, Qtr3dLightSource *light)
{
    setProgram(Qtr3d::DefaultLighting);
    drawBuffers(perspectiveMatrix,worldMatrix, light);
}

// Simple helper to make a single buffer object.
//-------------------------------------------------------------------------------------------------
GLuint Qtr3dShader::makeBO(void* data, GLsizei size, GLenum type, int accessFlags ) {
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    GLuint bo;
    f->glGenBuffers( 1, &bo );
    f->glBindBuffer( type, bo );
    f->glBufferData( type, size, data, accessFlags );

    return( bo );
}

//-------------------------------------------------------------------------------------------------
void Qtr3dShader::onProgramChange()
{
}

//-------------------------------------------------------------------------------------------------
void Qtr3dShader::setProgram(Qtr3d::LightingType lightType)
{
    if (mCurrentType == lightType)
        return;

    mCurrentType = lightType;
    if (!mShaders.contains(mCurrentType))
        return;
    currentProgram()->bind();
    onProgramChange();
}

//-------------------------------------------------------------------------------------------------
QOpenGLShaderProgram *Qtr3dShader::currentProgram()
{
    Q_ASSERT(mShaders.contains(mCurrentType));
    return mShaders[mCurrentType].program;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dShader::initShader(Qtr3d::LightingType lightType, const QString &eglFile)
{
    auto *vertexShader   = new QOpenGLShader(QOpenGLShader::Vertex);
    auto *fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
    auto *program        = new QOpenGLShaderProgram();

    QString extension;
    switch(lightType) {
    case Qtr3d::NoLighting:    extension = "nolight";     break;
    case Qtr3d::FlatLighting:  extension = "flatlight";   break;
    case Qtr3d::PhongLighting: extension = "phonglight";  break;
    default:break;
    }
    Q_ASSERT(!extension.isEmpty());

    vertexShader->compileSourceFile(QString(":/%1_%2.vert").arg(eglFile).arg(extension));
    Q_ASSERT(vertexShader->isCompiled());

    fragmentShader->compileSourceFile(QString(":/%1_%2.frag").arg(eglFile).arg(extension));
    Q_ASSERT(fragmentShader->isCompiled());

    program->addShader(vertexShader);
    program->addShader(fragmentShader);

    bool done = program->link();
    Q_ASSERT_X(done,"Shader Linker", program->log().toUtf8().data());

    mShaders[lightType].program        = program;
    mShaders[lightType].vertexShader   = vertexShader;
    mShaders[lightType].fragmentShader = fragmentShader;
}

