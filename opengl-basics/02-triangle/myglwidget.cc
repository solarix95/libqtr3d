#include <QOpenGLFunctions>
#include <QDebug>
#include "myglwidget.h"

static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
};

//-------------------------------------------------------------------------------------------------
MyGlWidget::MyGlWidget()
{
}

//-------------------------------------------------------------------------------------------------
void MyGlWidget::initializeGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->initializeOpenGLFunctions();

    f->glEnable(GL_DEPTH_TEST);
    f->glDepthFunc(GL_LESS);

    setupShader();

    f->glGenBuffers(1, &mVertexbufferId);
    f->glBindBuffer(GL_ARRAY_BUFFER, mVertexbufferId);
    f->glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    qDebug() << "OpenGL initialized";
}

//-------------------------------------------------------------------------------------------------
void MyGlWidget::paintGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    f->glClearColor(0.0 /*red*/,0.0 /*green*/,0.0 /*blue*/,  1.0f ) ;

    mShaderProgram->bind();


    f->glEnableVertexAttribArray(0);
    f->glBindBuffer(GL_ARRAY_BUFFER, mVertexbufferId);
    f->glVertexAttribPointer(
                0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
                );

    // Draw the triangle !
    f->glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

    f->glDisableVertexAttribArray(0);

    qDebug() << "OpenGL paint";
}

//-------------------------------------------------------------------------------------------------
void MyGlWidget::setupShader()
{
    mVertexShader   = new QOpenGLShader(QOpenGLShader::Vertex);
    mFragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);

    mVertexShader->compileSourceFile(":/SimpleVertexShader.vert");
    mFragmentShader->compileSourceFile(":/SimpleFragmentShader.frag");


    Q_ASSERT(mVertexShader->isCompiled());
    Q_ASSERT(mFragmentShader->isCompiled());

    mShaderProgram = new QOpenGLShaderProgram();
    mShaderProgram->addShader(mVertexShader);
    mShaderProgram->addShader(mFragmentShader);

    bool done = mShaderProgram->link();
    Q_ASSERT(done);
}
