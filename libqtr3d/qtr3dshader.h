#ifndef QTR3DSHADER_H
#define QTR3DSHADER_H

#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QMap>

#include "qtr3dtypes.h"

class Qtr3dMesh;
class Qtr3dGeometryBufferState;
class Qtr3dCamera;
class Qtr3dLightSource;

//-------------------------------------------------------------------------------------------------
class Qtr3dShader : public QObject
{
public:
    Qtr3dShader(QObject *parent, const QString &eglFile, const QString &fragVaryingCode, const QString &fragColorCode);

    void setDefaultLighting(Qtr3d::LightingType l);
    inline Qtr3d::LightingType defaultLighting() const { return mDefaultLighting; }

    void setProgram(Qtr3d::LightingType lightType);

    void render(const Qtr3dMesh &mesh, const QMatrix4x4 &modelView, const Qtr3dCamera &camera, Qtr3d::LightingType lighting, const Qtr3dLightSource &light);

    // static GLuint makeBO(void* data, GLsizei size, GLenum type = GL_ARRAY_BUFFER);
    static GLuint makeBO(void* data, GLsizei size, GLenum type = GL_ARRAY_BUFFER, int accessFlags = GL_STATIC_DRAW);

protected:
    virtual void onProgramChange();

    virtual void drawBuffer_NoLight(const Qtr3dMesh    &mesh, const QMatrix4x4 &modelView, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix) = 0;
    virtual void drawBuffer_FlatLight(const Qtr3dMesh  &mesh, const QMatrix4x4 &modelView, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, const Qtr3dLightSource &light) = 0;
    virtual void drawBuffer_PhongLight(const Qtr3dMesh &mesh, const QMatrix4x4 &modelView, const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, const Qtr3dLightSource &light) = 0;

    QOpenGLShaderProgram *currentProgram();

private:
    void initShader(Qtr3d::LightingType lightType, const QString &extension, const QString &fragVaryingCode, const QString &fragColorCode);
    static QByteArray shaderCode(const QString &name);

    struct ShaderComponents {
        QOpenGLShaderProgram *program;
        QOpenGLShader        *vertexShader;
        QOpenGLShader        *fragmentShader;
    };

    Qtr3d::LightingType         mDefaultLighting;

    QMap<int, ShaderComponents> mShaders;
    Qtr3d::LightingType         mCurrentType;

};


#endif // QTR3DSHADER_H
