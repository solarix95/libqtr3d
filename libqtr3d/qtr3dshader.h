#ifndef QTR3DSHADER_H
#define QTR3DSHADER_H

#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QMap>

#include "qtr3dtypes.h"
#include "qtr3dlightsource.h"

//-------------------------------------------------------------------------------------------------
class Qtr3dShader : public QObject
{
public:
    Qtr3dShader(const QString &eglFile);

    void setDefaultLighting(Qtr3d::LightingType l);
    void render(const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, Qtr3dLightSource *light);

    // static GLuint makeBO(void* data, GLsizei size, GLenum type = GL_ARRAY_BUFFER);
    static GLuint makeBO(void* data, GLsizei size, GLenum type = GL_ARRAY_BUFFER, int accessFlags = GL_STATIC_DRAW);

protected:
    // static inline void matrixAsUniform(QOpenGLFunctions *f, GLuint location, QMatrix4x4 m) { f->glUniformMatrix4fv(location,1,GL_TRUE,m.data()); }
    virtual void drawBuffers(const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix, Qtr3dLightSource *light) = 0;
    virtual void onProgramChange();

    void setProgram(Qtr3d::LightingType lightType);
    QOpenGLShaderProgram *currentProgram();
    inline Qtr3d::LightingType defaultLighting() const { return mDefaultLighting; }
private:
    void initShader(Qtr3d::LightingType lightType, const QString &extension);

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
