#include <algorithm>
#include <QOpenGLFunctions>
#include <QTimer>
#include <QPainter>
#include <QResizeEvent>
#include <QSurfaceFormat>
#include <QMessageBox>

#include "qtr3dtexturefactory.h"
#include "qtr3dlightsource.h"
#include "qtr3dwidget.h"
#include "qtr3dcamera.h"
#include "qtr3dmodel.h"
#include "qtr3dmesh.h"
#include "qtr3dgeometry.h"
#include "qtr3dpointcloud.h"
#include "qtr3dmodelanimator.h"
#include "qtr3dassets.h"
#include "physics/qtr3dfpsloop.h"
#include "shaders/qtr3dshader.h"
#include "shaders/qtr3dtexturedmeshshader.h"
#include "shaders/qtr3dvertexmeshshader.h"
#include "shaders/qtr3dplainshader.h"
#include "shaders/qtr3dpcshader.h"

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
void Qtr3dWidget::setOptions(int ops)
{
    setOptions((Options)ops);
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
Qtr3dPointCloud *Qtr3dWidget::createPointCloud()
{
    makeCurrent();
    return assets()->createPointCloud();
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

    f->glEnable(GL_DEPTH_TEST);
    f->glDepthFunc(GL_LEQUAL);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dWidget::paint3D()
{

    // paintMeshes();
    // paintModels();
    // paintPointClouds();
    paintGeometries();
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

    mPlainShader        = new Qtr3dPlainShader(this);      handleShaderError("Plain",         mPlainShader);
    mVertexMeshShader   = new Qtr3dVertexMeshShader(this); handleShaderError("Colored Mesh",  mVertexMeshShader);
    mTexturedMeshShader = new Qtr3dTexturedShader(this);   handleShaderError("Textured Mesh", mTexturedMeshShader);
    mPointCloudShader   = new Qtr3dPcShader(this);         handleShaderError("Point Cloud",   mPointCloudShader);
    mTextures           = new Qtr3dTextureFactory();

    bool originRebasing = mOptions.testFlag(OriginRebasing);

    mPlainShader->setOriginRebasing(originRebasing);
    mVertexMeshShader->setOriginRebasing(originRebasing);
    mTexturedMeshShader->setOriginRebasing(originRebasing);
    mPointCloudShader->setOriginRebasing(originRebasing);

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
void Qtr3dWidget::paintGeometries()
{
    QList<Qtr3dGeometry*> frontList;
    QList<Qtr3dGeometry*> regularList;
    QList<Qtr3dGeometry*> zOrderedList;

    const auto &rootList = assets()->geometries();
    for(auto *geometry: rootList) {
        if (geometry->bufferStates().isEmpty())
            continue;

        Qtr3dShader *shader = nullptr;
        switch (geometry->shader()) {
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

        // Background-Option overrides all others. Blending is not possible. Usecase: Skybox
        if (geometry->renderOptions().testFlag(Qtr3d::BackgroundOption)) {
            frontList << geometry;
            continue;
        }

        if (geometry->renderOptions().testFlag(Qtr3d::ZOrderOption)) {
            zOrderedList << geometry;
            continue;
        }

        regularList << geometry;
    }


    for (auto *geometry: frontList)
        renderGeometry(geometry);

    for (auto *geometry: regularList)
        renderGeometry(geometry);

    QList<pQtr3dStateZ> blendRenderPipeline;
    bool originRebasing = mOptions.testFlag(OriginRebasing);
    QMatrix4x4 worldView = camera()->worldView(originRebasing ? false:true);
    for (auto *geometry: zOrderedList) {
        for (auto *state: geometry->bufferStates()) {

            /*

            TODO

            QVector3D previewCenter = worldView.map(state->pos());
            if (previewCenter.z() > state->radius())
                continue;
            */
            blendRenderPipeline << pQtr3dStateZ((state->pos() - camera()->pos()).length(),state);
        }
    }
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
        shader->render(*mesh,zInfo.state->modelView(originRebasing ? camera()->pos() : Qtr3dDblVector3D{0,0,0}),
                       QVector<QMatrix4x4>(), *camera(),
                       nextLightingTyp,*primaryLightSource(), assets()->environment());
    }
}


#if 0
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

            if (mesh->hasRenderOption(Qtr3dGeometry::ZOrderOption)) {
                blendRenderPipeline << pQtr3dStateZ((state->pos() - camera()->pos()).length(),state);
                continue;
            }

            auto nextLightingTyp = state->lightingType();
            if (nextLightingTyp == Qtr3d::DefaultLighting)
                nextLightingTyp = shader->defaultLighting();


            shader->render(*mesh,state->modelView().toFloat(),QVector<QMatrix4x4>(), *camera(),nextLightingTyp,*primaryLightSource(), assets()->environment());
        }
    }

    // now Render the Z-Ordered Blending Pipeline
    // qSort(blendRenderPipeline.begin(), blendRenderPipeline.end(), stateZLessThan);
    std::sort(blendRenderPipeline.begin(), blendRenderPipeline.end(), stateZLessThan);

    for(const auto &zInfo: blendRenderPipeline) {
        const auto &geometry = zInfo.state->buffer();
        const auto *mesh = dynamic_cast<const Qtr3dMesh*>(&geometry);
        if (!mesh)
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

        Q_ASSERT(shader);
        auto nextLightingTyp = zInfo.state->lightingType();
        if (nextLightingTyp == Qtr3d::DefaultLighting)
            nextLightingTyp = shader->defaultLighting();
        shader->render(*mesh,zInfo.state->modelView().toFloat(),QVector<QMatrix4x4>(), *camera(),nextLightingTyp,*primaryLightSource(), assets()->environment());
    }
}

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

void Qtr3dWidget::paintPointClouds()
{
    const auto &clouds = assets()->pointClouds();
    for(auto *cloud: clouds) {

        const auto &states = cloud->bufferStates();

        if (states.isEmpty())
            continue;

        for(auto *state: states) {
            if (!state->enabled())
                continue;

            /*
            QVector3D previewCenter = camera()->worldMatrix().map(state->pos());
            if (previewCenter.z() > 0) {
                continue;
            }
            */
            mPointCloudShader->setProgram(Qtr3d::NoLighting);
            mPointCloudShader->render(*cloud,state->modelView().toFloat(),QVector<QMatrix4x4>(), *camera(),Qtr3d::NoLighting,*primaryLightSource(), assets()->environment());
        }
    }
}

#endif

//-------------------------------------------------------------------------------------------------
void Qtr3dWidget::renderGeometry(Qtr3dGeometry *buffer)
{
    Qtr3dShader *shader = nullptr;
    switch (buffer->shader()) {
    case Qtr3d::PlainShader:
        shader = mPlainShader; break;
    case Qtr3d::ColoredShader:
        shader = mVertexMeshShader; break;
    case Qtr3d::TexturedShader:
        shader = mTexturedMeshShader; break;
    default: break;
    }

    Q_ASSERT(shader);

    const auto &states = buffer->bufferStates();
    shader->setProgram(Qtr3d::DefaultLighting);

    bool originRebasing = mOptions.testFlag(OriginRebasing);
    QMatrix4x4 worldView = camera()->worldView(originRebasing ? false:true);

    for(auto *state: states) {
        if (!state->enabled())
            continue;

        /*

        TODO: Adjust code to rebase

        QVector3D previewCenter = camera()->worldView().map(state->pos()); // worldView.map(state->pos()); //
        if (previewCenter.z() > state->radius())
            continue;
        */

        auto nextLightingTyp = state->lightingType();
        if (nextLightingTyp == Qtr3d::DefaultLighting)
            nextLightingTyp = shader->defaultLighting();

        switch(buffer->pipeLine()) {
        case Qtr3dGeometry::MeshPipeline:
            shader->render(*(Qtr3dMesh*)buffer,state->modelView(originRebasing ? camera()->pos() : Qtr3dDblVector3D{0,0,0}),QVector<QMatrix4x4>(), *camera(),
                           nextLightingTyp,*primaryLightSource(), assets()->environment());
            break;
        case Qtr3dGeometry::PointCloudPipeline:
            mPointCloudShader->setProgram(Qtr3d::NoLighting);
            mPointCloudShader->render(*(Qtr3dPointCloud*)buffer,state->modelView(),QVector<QMatrix4x4>(), *camera(),Qtr3d::NoLighting,*primaryLightSource(), assets()->environment());
            break;
         case Qtr3dGeometry::ModelPipeline:
            if (!state->animator())
                renderStaticModel(*(Qtr3dModel*)buffer,state);
            else {
                renderAnimatedModel(*(Qtr3dModel*)buffer,state);
            }
            break;
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Qtr3dWidget::renderStaticModel(const Qtr3dModel &model, Qtr3dGeometryState *state)
{
    bool originRebasing = mOptions.testFlag(OriginRebasing);
    QMatrix4x4 worldView = camera()->worldView(originRebasing ? false:true);

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

            // for (int i=0; i<mesh->vertexCount(); i++) {
            //    qDebug() << i << mesh->vertex(i).p.toQVector();
            //}
            shader->render(*mesh,state->modelView((originRebasing ? camera()->pos() : Qtr3dDblVector3D{0,0,0})) * node->translation(), QVector<QMatrix4x4>(),*camera(),
                           nextLightingTyp,*primaryLightSource(), assets()->environment());
        }
    }
}

//-------------------------------------------------------------------------------------------------
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

            QMatrix4x4 rootTransform;
            rootTransform.setToIdentity();

            QMatrix4x4 globalInverseTransform; // = nodes.mRootNode->mTranslation.inverted();
            globalInverseTransform.setToIdentity();
            Qtr3dModel::setupSkeleton(skeleton,node->rootNode(),mesh, state->animator(),rootTransform, globalInverseTransform);

            shader->render(*mesh,state->modelView(), skeleton,*camera(),nextLightingTyp,*primaryLightSource(), assets()->environment());
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Qtr3dWidget::handleShaderError(const QString &name, Qtr3dShader *shader)
{
    if (shader->lastError().isEmpty())
        return;
    QMessageBox::warning(this,QString("Shader: '%1'").arg(name), QString("Shader Error: \n%1").arg(shader->lastError()));
}

