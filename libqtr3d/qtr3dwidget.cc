#include <QOpenGLFunctions>
#include <QTimer>
#include <QResizeEvent>
#include <QSurfaceFormat>

#include "qtr3dgeometrybufferfactory.h"
#include "qtr3dtexturedmeshshader.h"
#include "qtr3dvertexmeshshader.h"
#include "qtr3dtexturefactory.h"
#include "qtr3dtexturedquad.h"
#include "qtr3dlightsource.h"
#include "qtr3dplainshader.h"
#include "qtr3dwidget.h"
#include "qtr3dshader.h"
#include "qtr3dcamera.h"
#include "qtr3dmodel.h"
#include "qtr3dcontext.h"

//-------------------------------------------------------------------------------------------------
Qtr3dWidget::Qtr3dWidget(Options ops, QWidget *parent)
    : QOpenGLWidget(parent)
    , mOptions(ops)
    , mCamera(nullptr)
    , mFactory(nullptr)
    , mLightSource(nullptr)
    , mContext(nullptr)
    , mPlainShader(nullptr)
    , mTexturedMeshShader(nullptr)
    , mClearColor(Qt::black)
{
    preInitializing();
}

//-------------------------------------------------------------------------------------------------
Qtr3dWidget::Qtr3dWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , mOptions(NoOption)
    , mCamera(nullptr)
    , mFactory(nullptr)
    , mLightSource(nullptr)
    , mContext(nullptr)
    , mPlainShader(nullptr)
    , mTexturedMeshShader(nullptr)
    , mClearColor(Qt::black)
{
    preInitializing();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dWidget::setClearColor(QColor c)
{
    mClearColor = c;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dWidget::setDefaultLighting(Qtr3d::LightingType t)
{
    Q_ASSERT(mVertexMeshShader);
    mVertexMeshShader->setDefaultLighting(t);

    Q_ASSERT(mTexturedMeshShader);
    mTexturedMeshShader->setDefaultLighting(t);

    Q_ASSERT(mPlainShader);
    mPlainShader->setDefaultLighting(t);
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
Qtr3dContext *Qtr3dWidget::bufferContext()
{
    return mContext;
}

//-------------------------------------------------------------------------------------------------
Qtr3dGeometryBufferFactory *Qtr3dWidget::factory()
{
    return mFactory;
}

//-------------------------------------------------------------------------------------------------
Qtr3dLightSource *Qtr3dWidget::primaryLightSource()
{
    if (!mLightSource)
        mLightSource = new Qtr3dLightSource({0,0,0},Qt::white);
    return mLightSource;
}

//-------------------------------------------------------------------------------------------------
Qtr3dTexturedMesh *Qtr3dWidget::createTexturedMesh(const QString &textureName)
{
    makeCurrent();
    return factory()->createTexturedMesh(textureName);
}

//-------------------------------------------------------------------------------------------------
Qtr3dMesh *Qtr3dWidget::createMesh()
{
    makeCurrent();
    return bufferContext()->createMesh();
}

//-------------------------------------------------------------------------------------------------
Qtr3dModel *Qtr3dWidget::createModel()
{
    makeCurrent();
    return bufferContext()->createModel();
}

//-------------------------------------------------------------------------------------------------
Qtr3dGeometryBufferState *Qtr3dWidget::createState(Qtr3dGeometryBuffer *buffer, Qtr3d::LightingType ltype)
{
    return bufferContext()->createState(buffer, ltype);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dWidget::updateRequested()
{
    update();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dWidget::initializeGL()
{
    /*
    if (context()->shareContext() != mContext) {
        context()->setShareContext(mContext);
        mContext->create();
        return;
    }
    */
    // mContext->create();

    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->initializeOpenGLFunctions();

    // f->glEnable(GL_MULTISAMPLE); IMHO obsolete..
    f->glEnable(GL_DEPTH_TEST);
    f->glDepthFunc(GL_LEQUAL);

    mPlainShader        = new Qtr3dPlainShader(this);
    mVertexMeshShader   = new Qtr3dVertexMeshShader(this);
    mTexturedMeshShader = new Qtr3dTexturedShader(this);
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

    paintMeshes();
    paintModels();

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
void Qtr3dWidget::preInitializing()
{
    if (!mContext)
        mContext = new Qtr3dContext(this);

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

//-------------------------------------------------------------------------------------------------
void Qtr3dWidget::paintMeshes()
{
    const QList<Qtr3dMesh*> rootBuffers = bufferContext()->meshes();

    for(auto *mesh: rootBuffers) {
        if (mesh->bufferStates().isEmpty())
            continue;

        Qtr3dShader *shader = nullptr;
        switch (mesh->shader()) {
        case Qtr3d::PlainShader:
            shader = mPlainShader; break;
        case Qtr3d::ColoredShader:
            shader = mVertexMeshShader; break;
        case Qtr3d::TexturedShader:
            shader = mTexturedMeshShader; break;
        default: break;
        }

        if (!shader)
            continue;

        const Qtr3dGeometryBufferStates &states = mesh->bufferStates();
        shader->setProgram(Qtr3d::DefaultLighting);
        for(auto *state: states) {
            if (!state->enabled())
                continue;
            auto nextLightingTyp = state->lightingType();
            if (nextLightingTyp == Qtr3d::DefaultLighting)
                nextLightingTyp = shader->defaultLighting();
            shader->render(*mesh,state->modelView(),*camera(),nextLightingTyp,*primaryLightSource());
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Qtr3dWidget::paintModels()
{
    const QList<Qtr3dModel*> models = bufferContext()->models();

    for(auto *model: models) {

        const Qtr3dGeometryBufferStates &states = model->bufferStates();
        const Qtr3dModelNodes           &nodes  = model->nodes();

        if (states.isEmpty() || nodes.isEmpty())
            continue;

        for(auto *state: states) {
            if (!state->enabled())
                continue;
            for(auto *node: nodes) {
                Qtr3dShader *shader = nullptr;
                switch (node->mMesh->shader()) {
                case Qtr3d::PlainShader:
                    shader = mPlainShader; break;
                case Qtr3d::ColoredShader:
                    shader = mVertexMeshShader; break;
                case Qtr3d::TexturedShader:
                    shader = mTexturedMeshShader; break;
                default: break;
                }

                if (!shader)
                    continue;

                shader->setProgram(Qtr3d::DefaultLighting);
                auto nextLightingTyp = state->lightingType();
                if (nextLightingTyp == Qtr3d::DefaultLighting)
                    nextLightingTyp = shader->defaultLighting();

                shader->render(*node->mMesh,state->modelView() * node->translation(),*camera(),nextLightingTyp,*primaryLightSource());
            }
        }
    }
}


