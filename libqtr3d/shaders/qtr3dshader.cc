
#include <QOpenGLFunctions>
#include <QFile>
#include "qtr3dshader.h"
#include "../qtr3dmesh.h"
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
Qtr3dShader::Qtr3dShader(QObject *parent, const QString &vertFile, const QString &fragFile)
 : QObject(parent)
 , mDefaultLighting(Qtr3d::NoLighting)
 , mCurrentType(Qtr3d::DefaultLighting)
{
    auto *vertexShader   = new QOpenGLShader(QOpenGLShader::Vertex);
    auto *fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
    auto *program        = new QOpenGLShaderProgram();


    if (!compileShader(vertexShader,fragmentShader,program,
                       shaderCode(QString(":/%1.vert").arg(vertFile)),
                       shaderCode(QString(":/%1.frag").arg(fragFile))))
    {
        return;
    }

    mShaders[Qtr3d::DefaultLighting].vertexShader   = vertexShader;
    mShaders[Qtr3d::DefaultLighting].fragmentShader = fragmentShader;
    mShaders[Qtr3d::DefaultLighting].program        = program;

    mShaders[Qtr3d::NoLighting].vertexShader   = vertexShader;
    mShaders[Qtr3d::NoLighting].fragmentShader = fragmentShader;
    mShaders[Qtr3d::NoLighting].program        = program;
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
void Qtr3dShader::releaseBO(GLuint &bufferId)
{
    if (!bufferId)
        return;
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glDeleteBuffers(1,&bufferId);
    bufferId = 0;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dShader::onProgramChange()
{
}

//-------------------------------------------------------------------------------------------------
void Qtr3dShader::drawBuffer_NoLight(const Qtr3dMesh &mesh, const QMatrix4x4 &modelView, const QVector<QMatrix4x4> &meshSkeleton, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix)
{
    Q_ASSERT(0 && "not implemented");
}

//-------------------------------------------------------------------------------------------------
void Qtr3dShader::drawBuffer_FlatLight(const Qtr3dMesh &mesh, const QMatrix4x4 &modelView, const QVector<QMatrix4x4> &meshSkeleton, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, const Qtr3dLightSource &light)
{
    Q_ASSERT(0 && "not implemented");
}

//-------------------------------------------------------------------------------------------------
void Qtr3dShader::drawBuffer_PhongLight(const Qtr3dMesh &mesh, const QMatrix4x4 &modelView, const QVector<QMatrix4x4> &meshSkeleton, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, const Qtr3dLightSource &light)
{
    Q_ASSERT(0 && "not implemented");
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
void Qtr3dShader::render(const Qtr3dMesh &mesh, const QMatrix4x4 &modelView, const QVector<QMatrix4x4> &meshSkeleton, const QMatrix4x4 &worldView, const QMatrix4x4 &projection,
                         Qtr3d::LightingType lighting, const Qtr3dLightSource &light, const Qtr3dEnvironment &env)
{
    if (mesh.meshType() < Qtr3d::Triangle) // No Lighing for dots and lines... otherwise you can't see basic shapes in complex models...
        lighting = Qtr3d::NoLighting;

    setProgram(lighting);

    currentProgram()->setUniformValue("fog.color",     env.clearColor4f());
    currentProgram()->setUniformValue("fog.distance",  env.fogDistance());
    currentProgram()->setUniformValue("pushToBack",    GLint(mesh.renderOptions().testFlag(Qtr3d::BackgroundOption) ? 1:0));

    switch(lighting) {
    case Qtr3d::NoLighting:
        drawBuffer_NoLight(mesh, modelView, meshSkeleton, projection,worldView);
        break;
    case Qtr3d::FlatLighting:
        drawBuffer_FlatLight(mesh, modelView, meshSkeleton, projection, worldView, light);
        break;
    case Qtr3d::PhongLighting:
        drawBuffer_PhongLight(mesh, modelView, meshSkeleton, projection, worldView, light);
        break;
    default:break;
    }
}


//-------------------------------------------------------------------------------------------------
QOpenGLShaderProgram *Qtr3dShader::currentProgram()
{
    if (mShaders.contains(mCurrentType))
        return mShaders[mCurrentType].program;
    return mShaders[Qtr3d::DefaultLighting].program;
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


    if (!compileShader(vertexShader,fragmentShader, program,
                       shaderCode(QString(":/%1.vert").arg(eglFile)),
                       shaderCode(QString(":/%1.frag").arg(lighting))
                                                             .replace("#QTR3d_SHADER_PASS", fragVaryingCode.toUtf8())
                                                             .replace("#QTR3D_FRAGMENT_COLOR",fragColorCode.toUtf8())))
    {
        return;
    }

    mShaders[lightType].program        = program;
    mShaders[lightType].vertexShader   = vertexShader;
    mShaders[lightType].fragmentShader = fragmentShader;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dShader::compileShader(QOpenGLShader *vShader, QOpenGLShader *fShader, QOpenGLShaderProgram *program, const QByteArray &vCode, const QByteArray &fCode)
{
    vShader->compileSourceCode(vCode);
    if (!vShader->isCompiled()) {
        mError = vShader->log();
        vShader->deleteLater(); fShader->deleteLater(); program->deleteLater();
        return false;
    }

    fShader->compileSourceCode(fCode);
    if (!fShader->isCompiled()) {
        mError = fShader->log();
        vShader->deleteLater(); fShader->deleteLater(); program->deleteLater();
        return false;
    }

    program->addShader(vShader);
    program->addShader(fShader);
    if (!program->link()) {
        mError = fShader->log();
        vShader->deleteLater(); fShader->deleteLater(); program->deleteLater();
        return false;
    }

    return true;
}

//-------------------------------------------------------------------------------------------------
QByteArray Qtr3dShader::shaderCode(const QString &name)
{
    QFile f(name);
    bool done = f.open(QIODevice::ReadOnly);
    Q_ASSERT(done);
    return f.readAll();
}

