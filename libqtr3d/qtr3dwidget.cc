#include <algorithm>
#include <QOpenGLFunctions>
#include <QTimer>
#include <QPainter>
#include <QResizeEvent>
#include <QSurfaceFormat>

#include "qtr3dtexturefactory.h"
#include "qtr3dlightsource.h"
#include "qtr3dwidget.h"
#include "qtr3dcamera.h"
#include "qtr3dmodel.h"
#include "qtr3dmesh.h"
#include "qtr3dmodelanimator.h"
#include "qtr3dassets.h"
#include "physics/qtr3dfpsloop.h"
#include "shaders/qtr3dshader.h"
#include "shaders/qtr3dtexturedmeshshader.h"
#include "shaders/qtr3dvertexmeshshader.h"
#include "shaders/qtr3dplainshader.h"

//-------------------------------------------------------------------------------------------------
struct pQtr3dStateZ {
    float z;
    Qtr3dGeometryState *state;
    pQtr3dStateZ(float az, Qtr3dGeometryState *as): z(az), state(as) {}
};

bool stateZLessThan(const pQtr3dStateZ &d1, const pQtr3dStateZ &d2)
{
    return d1.z > d2.z;
}

//-------------------------------------------------------------------------------------------------
Qtr3dWidget::Qtr3dWidget(Options ops, QWidget *parent)
    : QOpenGLWidget(parent)
    , mOptions(ops)
    , mCamera(nullptr)
    , mLightSource(nullptr)
    , mContext(nullptr)
    , mPlainShader(nullptr)
    , mTexturedMeshShader(nullptr)
{
    preInitializing();
}

//-------------------------------------------------------------------------------------------------
Qtr3dWidget::Qtr3dWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , mOptions(NoOption)
    , mCamera(nullptr)
    , mLightSource(nullptr)
    , mContext(nullptr)
    , mPlainShader(nullptr)
    , mTexturedMeshShader(nullptr)
{
    preInitializing();
}

//-------------------------------------------------------------------------------------------------
Qtr3dWidget::~Qtr3dWidget()
{
    if (mContext && (mContext->parent() == this)) {
        makeCurrent();
        mContext->reset();
        delete mContext;
    }
}

//-------------------------------------------------------------------------------------------------
void Qtr3dWidget::setOptions(Options ops)
{
    int samples = 0;
    if (ops.testFlag(MSAA4))
        samples = 4;
    else if (ops.testFlag(MSAA16))
        samples = 16;

    if (samples > 0) {
        QSurfaceFormat currentFormat = format();
        currentFormat.setSamples(samples);
        currentFormat.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
        setFormat(currentFormat);
    }

    mOptions = ops;
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
Qtr3dAssets *Qtr3dWidget::assets()
{
    return mContext;
}

//-------------------------------------------------------------------------------------------------
Qtr3dLightSource *Qtr3dWidget::primaryLightSource()
{
    if (!mLightSource)
        mLightSource = new Qtr3dLightSource({0,0,0},Qt::white);
    return mLightSource;
}

//-------------------------------------------------------------------------------------------------
Qtr3dMesh *Qtr3dWidget::createMesh()
{
    makeCurrent();
    return assets()->createMesh();
}

//-------------------------------------------------------------------------------------------------
Qtr3dModel *Qtr3dWidget::createModel()
{
    makeCurrent();
    return assets()->createModel();
}

//-------------------------------------------------------------------------------------------------
Qtr3dGeometryState *Qtr3dWidget::createState(Qtr3dGeometry *buffer, Qtr3d::LightingType ltype)
{
    return assets()->createState(buffer, ltype);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dWidget::updateRequested()
{
    if (assets()->loop().isActive())
        return;

    update();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dWidget::preparePainting()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QColor clearColor = assets()->environment().clearColor();
    f->glClearColor(clearColor.redF() ,  clearColor.greenF() ,  clearColor.blueF() ,  1.0f ) ;
    f->glEnable(GL_CULL_FACE) ;
    f->glCullFace(GL_BACK);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dWidget::paint3D()
{
    paintMeshes();
    paintModels();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dWidget::paint2D()
{
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

    f->glEnable(GL_DEPTH_TEST);
    f->glDepthFunc(GL_LEQUAL);

    mPlainShader        = new Qtr3dPlainShader(this);
    mVertexMeshShader   = new Qtr3dVertexMeshShader(this);
    mTexturedMeshShader = new Qtr3dTexturedShader(this);
    mTextures           = new Qtr3dTextureFactory();

    emit initialized();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dWidget::paintGL()
{
    preparePainting();

    // OpenGLs Meshes, Models, ...
    paint3D();

    // otherwise: can't see 2D Paintings..
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glDisable(GL_CULL_FACE);
    f->glDisable(GL_BLEND);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dWidget::paintEvent(QPaintEvent *e)
{
    QOpenGLWidget::paintEvent(e);
    paint2D();
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
    return new Qtr3dCamera(this);
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
        mContext = new Qtr3dAssets(this);

    setOptions(mOptions);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dWidget::paintMeshes()
{
    const auto rootBuffers = assets()->meshes();

    QList<pQtr3dStateZ> blendRenderPipeline;

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

        const auto &states = mesh->bufferStates();
        shader->setProgram(Qtr3d::DefaultLighting);
        for(auto *state: states) {
            if (!state->enabled())
                continue;

            QVector3D previewCenter = camera()->worldMatrix().map(state->pos());
            if (previewCenter.z() > state->radius())
                continue;

            if (mesh->blending()) {
                blendRenderPipeline << pQtr3dStateZ((state->pos() - camera()->pos()).length(),state);
                continue;
            }

            auto nextLightingTyp = state->lightingType();
            if (nextLightingTyp == Qtr3d::DefaultLighting)
                nextLightingTyp = shader->defaultLighting();


            shader->render(*mesh,state->modelView(),QVector<QMatrix4x4>(), *camera(),nextLightingTyp,*primaryLightSource(), assets()->environment());
        }
    }

    // now Render the Z-Ordered Blending Pipeline
    // qSort(blendRenderPipeline.begin(), blendRenderPipeline.end(), stateZLessThan);
    std::sort(blendRenderPipeline.begin(), blendRenderPipeline.end(), stateZLessThan);

    for(const auto &zInfo: blendRenderPipeline) {
        const auto &geometry = zInfo.state->buffer();
        const auto *mesh = dynamic_cast<const Qtr3dMesh*>(&geometry);
        Q_ASSERT(mesh);

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

        Q_ASSERT(shader);
        auto nextLightingTyp = zInfo.state->lightingType();
        if (nextLightingTyp == Qtr3d::DefaultLighting)
            nextLightingTyp = shader->defaultLighting();
        shader->render(*mesh,zInfo.state->modelView(),QVector<QMatrix4x4>(), *camera(),nextLightingTyp,*primaryLightSource(), assets()->environment());
    }
}

//-------------------------------------------------------------------------------------------------
void Qtr3dWidget::paintModels()
{
    const auto &models = assets()->models();

    for(auto *model: models) {

        const auto &states = model->bufferStates();
        const auto &nodes  = model->nodes();

        if (states.isEmpty() || nodes.isEmpty())
            continue;

        for(auto *state: states) {
            if (!state->enabled())
                continue;

            QVector3D previewCenter = camera()->worldMatrix().map(state->pos());
            if (previewCenter.z() > 0) {
                continue;
            }

            if (!state->animator())
                renderStaticModel(*model,state);
            else {
                renderAnimatedModel(*model,state);
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Qtr3dWidget::renderStaticModel(const Qtr3dModel &model, Qtr3dGeometryState *state)
{
    const auto &nodes  = model.nodes();
    for(auto *node: nodes.mNodes) {
        for (auto *mesh : node->mMeshes) {
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

            shader->setProgram(Qtr3d::DefaultLighting);
            auto nextLightingTyp = state->lightingType();
            if (nextLightingTyp == Qtr3d::DefaultLighting)
                nextLightingTyp = shader->defaultLighting();

            shader->render(*mesh,state->modelView() * node->translation(), QVector<QMatrix4x4>(),*camera(),nextLightingTyp,*primaryLightSource(), assets()->environment());
        }
    }
}

void Qtr3dWidget::renderAnimatedModel(const Qtr3dModel &model, Qtr3dGeometryState *state)
{
    const auto &nodes  = model.nodes();
    for(auto *node: nodes.mNodes) {
        for (auto *mesh : node->mMeshes) {
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

            shader->setProgram(Qtr3d::DefaultLighting);
            auto nextLightingTyp = state->lightingType();
            if (nextLightingTyp == Qtr3d::DefaultLighting)
                nextLightingTyp = shader->defaultLighting();

            QVector<QMatrix4x4> skeleton;
            skeleton.resize(mesh->bones().count());

            QMatrix4x4 rootTransform;
            rootTransform.setToIdentity();

            QMatrix4x4 globalInverseTransform = nodes.mRootNode->mTranslation.inverted();
            globalInverseTransform.setToIdentity();
            setupSkeleton(skeleton, nodes.mRootNode, mesh, state->animator(),rootTransform, globalInverseTransform);

            shader->render(*mesh,state->modelView(), skeleton,*camera(),nextLightingTyp,*primaryLightSource(), assets()->environment());
        }
    }
}

int Qtr3dWidget::setupSkeleton(QVector<QMatrix4x4> &skeleton, const Qtr3dModel::Node *node, const Qtr3dMesh *mesh, Qtr3dModelAnimator *animator, const QMatrix4x4 &parentTransform, const QMatrix4x4 &globalTransform)
{
    int updatesBones = 0;
    QMatrix4x4 nodePosition  = node->mTranslation;   // Default-Transform: Static Node Transform
    animator->transform(node->mName, nodePosition);  // If animated: override static transformation
    nodePosition = parentTransform * nodePosition;   // Tree transformation

    QVector4D tv(3,3,3,0);
    tv = nodePosition * tv;

    // Update Shader Mesh Skeleton
    if (!node->mName.isEmpty()) {
        int boneIndex = -1;
        const auto &bones = mesh->bones();
        for (int b=0; b<bones.count(); b++) {
            if (node->mName == bones[b].name) {
                boneIndex = b;
                break;
            }
        }
        if (boneIndex >= 0) {
            skeleton[boneIndex] = globalTransform * nodePosition * bones[boneIndex].offset;
            updatesBones++;
        }
    }

    for (const auto *child: node->mChilds)
        updatesBones += setupSkeleton(skeleton, child,mesh,animator,nodePosition, globalTransform);

    return updatesBones;
}


