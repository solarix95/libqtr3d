
#include <QOpenGLFunctions>
#include <QFile>
#include "qtr3dshader.h"
#include "../qtr3dcamera.h"
#include "../qtr3dlightsource.h"

//-------------------------------------------------------------------------------------------------
Qtr3dShader::Qtr3dShader(QObject *parent, const QString &eglFile, const QString &fragVaryingCode, const QString &fragColorCode)
 : QObject(parent)
 , mDefaultLighting(Qtr3d::NoLighting)
 , mCurrentType(Qtr3d::DefaultLighting)
{
    initShader(Qtr3d::NoLighting,    eglFile, fragVaryingCode, fragColorCode);
    initShader(Qtr3d::FlatLighting,  eglFile, fragVaryingCode, fragColorCode);
    initShader(Qtr3d::PhongLighting, eglFile, fragVaryingCode, fragColorCode);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dShader::setDefaultLighting(Qtr3d::LightingType l)
{
    mDefaultLighting = l;
    Q_ASSERT(mDefaultLighting != Qtr3d::DefaultLighting);
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
void Qtr3dShader::render(const Qtr3dMesh &mesh, const QMatrix4x4 &modelView, const Qtr3dCamera &camera, Qtr3d::LightingType lighting, const Qtr3dLightSource &light, const Qtr3dEnvironment &env)
{
    setProgram(lighting);

    currentProgram()->setUniformValue("fog.color",     env.clearColor4f());
    currentProgram()->setUniformValue("fog.distance",  env.fogDistance());

    switch(lighting) {
    case Qtr3d::NoLighting:
        drawBuffer_NoLight(mesh, modelView, camera.projection() , camera.worldMatrix());
        break;
    case Qtr3d::FlatLighting:
        drawBuffer_FlatLight(mesh, modelView, camera.projection() , camera.worldMatrix(), light);
        break;
    case Qtr3d::PhongLighting:
        drawBuffer_PhongLight(mesh, modelView, camera.projection() , camera.worldMatrix(), light);
        break;
    default:break;
    }
}

//-------------------------------------------------------------------------------------------------
QOpenGLShaderProgram *Qtr3dShader::currentProgram()
{
    Q_ASSERT(mShaders.contains(mCurrentType));
    return mShaders[mCurrentType].program;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dShader::initShader(Qtr3d::LightingType lightType, const QString &eglFile, const QString &fragVaryingCode, const QString &fragColorCode)
{
    auto *vertexShader   = new QOpenGLShader(QOpenGLShader::Vertex);
    auto *fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
    auto *program        = new QOpenGLShaderProgram();

    QString lighting;
    switch(lightType) {
    case Qtr3d::NoLighting:    lighting = "nolight";     break;
    case Qtr3d::FlatLighting:  lighting = "flatlight";   break;
    case Qtr3d::PhongLighting: lighting = "phonglight";  break;
    default:break;
    }
    Q_ASSERT(!lighting.isEmpty());

    vertexShader->compileSourceCode(shaderCode(QString(":/%1.vert").arg(eglFile)));
    Q_ASSERT(vertexShader->isCompiled());

    fragmentShader->compileSourceCode(shaderCode(QString(":/%1.frag").arg(lighting))
                                      .replace("#QTR3d_SHADER_PASS", fragVaryingCode.toUtf8())
                                      .replace("#QTR3D_FRAGMENT_COLOR",fragColorCode.toUtf8()));

    Q_ASSERT(fragmentShader->isCompiled());

    program->addShader(vertexShader);
    program->addShader(fragmentShader);

    bool done = program->link();
    Q_ASSERT_X(done,"Shader Linker", program->log().toUtf8().data());

    mShaders[lightType].program        = program;
    mShaders[lightType].vertexShader   = vertexShader;
    mShaders[lightType].fragmentShader = fragmentShader;
}

//-------------------------------------------------------------------------------------------------
QByteArray Qtr3dShader::shaderCode(const QString &name)
{
    QFile f(name);
    bool done = f.open(QIODevice::ReadOnly);
    Q_ASSERT(done);
    return f.readAll();
}

