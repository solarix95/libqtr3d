#include <QOpenGLFunctions>
#include <QTimer>
#include <QResizeEvent>
#include <QSurfaceFormat>

#include "qtr3dwidget.h"
#include "qtr3dshader.h"
#include "qtr3dcamera.h"
#include "qtr3dtexturedquad.h"
#include "qtr3dtexturedmeshshader.h"
#include "qtr3dvertexmeshshader.h"
#include "qtr3dtexturefactory.h"
#include "qtr3dgeometrybufferfactory.h"
#include "qtr3dmodel.h"

//-------------------------------------------------------------------------------------------------
Qtr3dWidget::Qtr3dWidget(Options ops, QWidget *parent)
    : QOpenGLWidget(parent)
    , mOptions(ops)
    , mCamera(nullptr)
    , mFactory(nullptr)
    , mTexturedMeshShader(nullptr)
    , mClearColor("#000000") // black
{
    initializeMultisampleAntiAliasing();
}

//-------------------------------------------------------------------------------------------------
Qtr3dWidget::Qtr3dWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , mOptions(NoOption)
    , mCamera(nullptr)
    , mFactory(nullptr)
    , mTexturedMeshShader(nullptr)
    , mClearColor("#000000") // black
{
    initializeMultisampleAntiAliasing();
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
Qtr3dGeometryBufferFactory *Qtr3dWidget::factory()
{
    return mFactory;
}

//-------------------------------------------------------------------------------------------------
Qtr3dTexturedMesh *Qtr3dWidget::createTexturedMesh(const QString &textureName)
{
    makeCurrent();
    return factory()->createTexturedMesh(textureName);
}

//-------------------------------------------------------------------------------------------------
Qtr3dVertexMesh *Qtr3dWidget::createVertexMesh()
{
    makeCurrent();
    return factory()->createVertexMesh();
}

//-------------------------------------------------------------------------------------------------
Qtr3dModel *Qtr3dWidget::createModel()
{
    makeCurrent();
    return factory()->createModel();
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

    // f->glEnable(GL_MULTISAMPLE); IMHO obsolete..
    f->glEnable(GL_DEPTH_TEST);
    f->glDepthFunc(GL_LEQUAL);

    mVertexMeshShader   = new Qtr3dVertexMeshShader("vertexmesh");
    mTexturedMeshShader = new Qtr3dTexturedMeshShader("textquad");
    mTextures           = new Qtr3dTextureFactory();
    mFactory            = new Qtr3dGeometryBufferFactory();

    mFactory->init(mVertexMeshShader,mTexturedMeshShader,mTextures);

    emit initialized();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dWidget::paintGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    f->glClearColor(mClearColor.redF() ,  mClearColor.greenF() ,  mClearColor.blueF() ,  1.0f ) ;
    f->glEnable(GL_CULL_FACE) ;
    f->glCullFace(GL_BACK);

    mVertexMeshShader->render(camera()->projection(), camera()->worldMatrix());
    mTexturedMeshShader->render(camera()->projection(),camera()->worldMatrix());

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

//-------------------------------------------------------------------------------------------------
//                                    PRIVATE
//-------------------------------------------------------------------------------------------------
void Qtr3dWidget::initializeMultisampleAntiAliasing()
{
    int samples = 0;
    if (mOptions.testFlag(MSAA4))
        samples = 4;
    else if (mOptions.testFlag(MSAA16))
        samples = 16;

    if (samples > 0) {
            QSurfaceFormat currentFormat = format();
            currentFormat.setSamples(samples);
            setFormat(currentFormat);
    }

}
