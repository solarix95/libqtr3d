#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>

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
};

#endif // MYGLWIDGET_H
