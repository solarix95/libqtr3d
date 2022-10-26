#ifndef QTR3DSHADER_H
#define QTR3DSHADER_H

#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>


//-------------------------------------------------------------------------------------------------
class Qtr3dShader : public QObject
{
public:
    Qtr3dShader(const QString &eglFile);

    void render(const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix);

    static GLuint makeBO(void* data, GLsizei size, GLenum type = GL_ARRAY_BUFFER);
    static GLuint makeBO(void* data, GLsizei size, GLenum type, int accessFlags);

protected:
    static inline void matrixAsUniform(QOpenGLFunctions *f, GLuint location, QMatrix4x4 m) { f->glUniformMatrix4fv(location,1,GL_TRUE,m.data()); }
    virtual void drawFlatBuffers(const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix);
    virtual void drawLightBuffers(const QMatrix4x4 &perspectiveMatrix, const QMatrix4x4 &worldMatrix);

    QOpenGLShaderProgram  *mShaderProgramFlat;
    QOpenGLShaderProgram  *mShaderProgramLight;

private:
    void initShader(const QString &name, QOpenGLShader *&vertex, QOpenGLShader *&fragment, QOpenGLShaderProgram *&program);

    QOpenGLShader         *mVertexShaderFlat;
    QOpenGLShader         *mFragmentShaderFlat;

    QOpenGLShader         *mVertexShaderLight;
    QOpenGLShader         *mFragmentShaderLight;
};


#endif // QTR3DSHADER_H
