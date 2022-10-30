#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QDebug>
#include <QTimer>
#include "myglwidget.h"


static const GLfloat g_vertex_buffer_data[] = {
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
         1.0f, 1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
         1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
         1.0f,-1.0f,-1.0f,
         1.0f, 1.0f,-1.0f,
         1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
         1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
         1.0f,-1.0f, 1.0f,
         1.0f, 1.0f, 1.0f,
         1.0f,-1.0f,-1.0f,
         1.0f, 1.0f,-1.0f,
         1.0f,-1.0f,-1.0f,
         1.0f, 1.0f, 1.0f,
         1.0f,-1.0f, 1.0f,
         1.0f, 1.0f, 1.0f,
         1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
         1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
         1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
         1.0f,-1.0f, 1.0f
    };

    // Two UV coordinatesfor each vertex. They were created with Blender.
    static const GLfloat g_uv_buffer_data[] = {
        0.000059f, 1.0f-0.000004f,
        0.000103f, 1.0f-0.336048f,
        0.335973f, 1.0f-0.335903f,
        1.000023f, 1.0f-0.000013f,
        0.667979f, 1.0f-0.335851f,
        0.999958f, 1.0f-0.336064f,
        0.667979f, 1.0f-0.335851f,
        0.336024f, 1.0f-0.671877f,
        0.667969f, 1.0f-0.671889f,
        1.000023f, 1.0f-0.000013f,
        0.668104f, 1.0f-0.000013f,
        0.667979f, 1.0f-0.335851f,
        0.000059f, 1.0f-0.000004f,
        0.335973f, 1.0f-0.335903f,
        0.336098f, 1.0f-0.000071f,
        0.667979f, 1.0f-0.335851f,
        0.335973f, 1.0f-0.335903f,
        0.336024f, 1.0f-0.671877f,
        1.000004f, 1.0f-0.671847f,
        0.999958f, 1.0f-0.336064f,
        0.667979f, 1.0f-0.335851f,
        0.668104f, 1.0f-0.000013f,
        0.335973f, 1.0f-0.335903f,
        0.667979f, 1.0f-0.335851f,
        0.335973f, 1.0f-0.335903f,
        0.668104f, 1.0f-0.000013f,
        0.336098f, 1.0f-0.000071f,
        0.000103f, 1.0f-0.336048f,
        0.000004f, 1.0f-0.671870f,
        0.336024f, 1.0f-0.671877f,
        0.000103f, 1.0f-0.336048f,
        0.336024f, 1.0f-0.671877f,
        0.335973f, 1.0f-0.335903f,
        0.667969f, 1.0f-0.671889f,
        1.000004f, 1.0f-0.671847f,
        0.667979f, 1.0f-0.335851f
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

    f->glGenBuffers(1, &mUVbufferId);
    f->glBindBuffer(GL_ARRAY_BUFFER, mUVbufferId);
    f->glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

    mTexture = new QOpenGLTexture(QImage(":/texture.png").mirrored());
    mTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    mTexture->setMagnificationFilter(QOpenGLTexture::Linear);

    qDebug() << "OpenGL initialized";

    // Now start the Animation:
    QTimer *t = new QTimer(this);
    connect(t, &QTimer::timeout, this, [this](){
        mAngle += 1;
        update();
    });

    mAngle = 0;
    t->start(25);
}

//-------------------------------------------------------------------------------------------------
void MyGlWidget::paintGL()
{
    QMatrix4x4 projection;
    projection.perspective(45,width()/(float)height(),0.1,100);

    QMatrix4x4 view;
    view.lookAt({4,3,3} , {0,0,0}, { 0,1,0});

    QMatrix4x4 model( 1, 0, 0, 0,
                      0, 1, 0, 0,
                      0, 0, 1, 0,
                      0, 0, 0, 1);

    // Animation Rotation:
    model.rotate(mAngle,{0,1,0});
    model.rotate(mAngle,{1,0,0});
    model.rotate(mAngle,{0,0,1});

    QMatrix4x4 MVP = projection * view * model;

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    f->glClearColor(0.0 /*red*/,0.0 /*green*/,0.0 /*blue*/,  1.0f ) ;

    mShaderProgram->bind();

    auto normalviewMatrix = mShaderProgram->uniformLocation("MVP" );
    auto myTextureSampler = mShaderProgram->uniformLocation("myTextureSampler" );
    mShaderProgram->setUniformValue(normalviewMatrix,MVP);

    // Textures
    // mTexture->bind(myTextureSampler); // does not work..?
    f->glActiveTexture( GL_TEXTURE0 );
    f->glBindTexture( GL_TEXTURE_2D,mTexture->textureId());
    f->glUniform1i( myTextureSampler, 0 );

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

    f->glEnableVertexAttribArray(1);
    f->glBindBuffer(GL_ARRAY_BUFFER, mUVbufferId);
    f->glVertexAttribPointer(
                1,                  // attribute. No particular reason for 1, but must match the layout in the shader.
                2,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void*)0            // array buffer offset
                );

    // Draw the triangle !
    f->glDrawArrays(GL_TRIANGLES, 0, 3*12); // 3 indices starting at 0 -> 1 triangle

    f->glDisableVertexAttribArray(0);
    f->glDisableVertexAttribArray(1);

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
