#include <QOpenGLFunctions>
#include <QTimer>
#include <QResizeEvent>

#include "qtr3dwidget.h"
#include "qtr3dshader.h"
#include "qtr3dcamera.h"
#include "qtr3dtexturedquad.h"
#include "qtr3dtexturedquadshader.h"
#include "qtr3dvertexmeshshader.h"
#include "qtr3dtexturefactory.h"

//-------------------------------------------------------------------------------------------------
Qtr3dWidget::Qtr3dWidget()
    : mCamera(nullptr)
    , mTextures(nullptr)
    , mTexturedQuadShader(nullptr)
    , mClearColor("#000000") // black
{
}

//-------------------------------------------------------------------------------------------------
Qtr3dCamera *Qtr3dWidget::camera()
{
    if (!mCamera) {
        mCamera = createCamera();
        connect(mCamera, &Qtr3dCamera::changed, this, &Qtr3dWidget::updateRequested);
    }
    return mCamera;
}

//-------------------------------------------------------------------------------------------------
Qtr3dTextureFactory *Qtr3dWidget::textures()
{
    if (!mTextures) {
        mTextures = createTextureFactory();
    }
    return mTextures;
}

//-------------------------------------------------------------------------------------------------
Qtr3dTexturedQuad *Qtr3dWidget::createTexturedQuad(const QString &textureName)
{
    makeCurrent();
    Qtr3dTexturedQuad *geometryBuffer = new Qtr3dTexturedQuad(textures(),textureName);
    mTexturedQuadShader->registerBuffer(*geometryBuffer);
    return geometryBuffer;
}

//-------------------------------------------------------------------------------------------------
Qtr3dVertexMesh *Qtr3dWidget::createVertexMesh()
{
    makeCurrent();
    Qtr3dVertexMesh *mesh = new Qtr3dVertexMesh();
    mVertexMeshShader->registerBuffer(*mesh);
    return mesh;
}

//-------------------------------------------------------------------------------------------------
Qtr3dGeometryBufferState *Qtr3dWidget::createBufferState(Qtr3dGeometryBuffer *buffer, bool isFlat)
{
    Q_ASSERT(buffer);
    Qtr3dGeometryBufferState *state = new Qtr3dGeometryBufferState(isFlat);
    buffer->registerBufferState(state);
    return state;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dWidget::updateRequested()
{
    update();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dWidget::initializeGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->initializeOpenGLFunctions();

    f->glEnable(GL_DEPTH_TEST);
    f->glDepthFunc(GL_LEQUAL) ;
    f->glEnable(GL_CULL_FACE) ;
    f->glCullFace(GL_BACK);

    mVertexMeshShader   = new Qtr3dVertexMeshShader("vertexmesh");
    mTexturedQuadShader = new Qtr3dTexturedQuadShader("textquad");
    emit initialized();

    /*
    QTimer *t = new QTimer(this);
    connect(t, SIGNAL(timeout()), this, SLOT(update()));
    t->start(1000/30);
    */
}

//-------------------------------------------------------------------------------------------------
void Qtr3dWidget::paintGL()
{
    // qDebug() << "begin paint GL";
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    f->glClearColor(mClearColor.redF() ,  mClearColor.greenF() ,  mClearColor.blueF() ,  1.0f ) ;
    f->glEnable(GL_CULL_FACE) ;

    mVertexMeshShader->render(camera()->projection(), camera()->worldMatrix());
    mTexturedQuadShader->render(camera()->projection(),camera()->worldMatrix());
    // qDebug() << "end paint GL";

    f->glDisable(GL_CULL_FACE); // otherwise: can't see 2D Paintings..
}

//-------------------------------------------------------------------------------------------------
void Qtr3dWidget::resizeEvent(QResizeEvent *e)
{
    camera()->setGeometry(e->size().width(), e->size().height());
    QOpenGLWidget::resizeEvent(e);
}

//-------------------------------------------------------------------------------------------------
Qtr3dCamera *Qtr3dWidget::createCamera()
{
    return new Qtr3dCamera();
}

//-------------------------------------------------------------------------------------------------
Qtr3dTextureFactory *Qtr3dWidget::createTextureFactory()
{
    return new Qtr3dTextureFactory();
}
