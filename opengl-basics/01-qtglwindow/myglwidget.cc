#include <QOpenGLFunctions>
#include <QDebug>
#include "myglwidget.h"

MyGlWidget::MyGlWidget()
{

}

void MyGlWidget::initializeGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->initializeOpenGLFunctions();

    f->glEnable(GL_DEPTH_TEST);
    f->glDepthFunc(GL_LESS);

    qDebug() << "OpenGL initialized";
}

void MyGlWidget::paintGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    f->glClearColor(0.0 /*red*/,0.0 /*green*/,0.0 /*blue*/,  1.0f ) ;

    qDebug() << "OpenGL paint";
}
