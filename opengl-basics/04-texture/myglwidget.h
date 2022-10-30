#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

class MyGlWidget : public QOpenGLWidget
{
public:
    MyGlWidget();

protected:
    virtual void initializeGL();
    virtual void paintGL();

private:
    void setupShader();

    QOpenGLShader        *mVertexShader;
    QOpenGLShader        *mFragmentShader;
    QOpenGLShaderProgram *mShaderProgram;

    GLuint                mVertexbufferId;
    GLuint                mUVbufferId;
    QOpenGLTexture       *mTexture;

    // Animation
    float                 mAngle;
};

#endif // MYGLWIDGET_H
