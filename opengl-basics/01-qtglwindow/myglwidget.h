#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLWidget>

class MyGlWidget : public QOpenGLWidget
{
public:
    MyGlWidget();

protected:
    virtual void initializeGL();
    virtual void paintGL();
};

#endif // MYGLWIDGET_H
