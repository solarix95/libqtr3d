
#include "qtr3dassets.h"
#include "qtr3dmesh.h"
#include "qtr3dmodel.h"
#include "qtr3dpointcloud.h"
#include "physics/qtr3dfpsloop.h"
#include "physics/qtr3dabstractspace.h"

//-------------------------------------------------------------------------------------------------
Qtr3dAssets::Qtr3dAssets(QObject *parent)
 : QOpenGLContext(parent)
 , mLoop(nullptr)
 , mSpace(nullptr)
{
}

//-------------------------------------------------------------------------------------------------
void Qtr3dAssets::reset()
{
    qDeleteAll(mMeshes);
    qDeleteAll(mModels);
    mMeshes.clear();
    mModels.clear();
}

//-------------------------------------------------------------------------------------------------
Qtr3dMesh *Qtr3dAssets::createMesh(bool root)
{
    auto *ret = new Qtr3dMesh(this);
    return root ? registerMesh(ret) : ret;
}

//-------------------------------------------------------------------------------------------------
Qtr3dModel *Qtr3dAssets::createModel()
{
    return registerModel(new Qtr3dModel(this));
}

//-------------------------------------------------------------------------------------------------
Qtr3dPointCloud *Qtr3dAssets::createPointCloud()
{
    return registerPointCloud(new Qtr3dPointCloud(this));
}

//-------------------------------------------------------------------------------------------------
void Qtr3dAssets::setLoop(Qtr3dFpsLoop *loop)
{
    Q_ASSERT(loop);
    if (mLoop)
        mLoop->deleteLater();
    mLoop = loop;
    connect(mLoop, &Qtr3dFpsLoop::step, &space(), &Qtr3dAbstractSpace::process, Qt::UniqueConnection);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dAssets::setSpace(Qtr3dAbstractSpace *space)
{
    Q_ASSERT(space);
    if (mSpace)
        mSpace->deleteLater();
    mSpace = space;
    connect(&loop(), &Qtr3dFpsLoop::step,mSpace, &Qtr3dAbstractSpace::process, Qt::UniqueConnection);
}

//-------------------------------------------------------------------------------------------------
Qtr3dGeometryState *Qtr3dAssets::createState(Qtr3dGeometry *buffer, Qtr3d::LightingType ltype)
{
    auto *ret = new Qtr3dGeometryState(buffer, ltype);
    buffer->registerBufferState(ret);
    return ret;
}

//-------------------------------------------------------------------------------------------------
const QList<Qtr3dMesh*> &Qtr3dAssets::meshes() const
{
    return mMeshes;
}

//-------------------------------------------------------------------------------------------------
const QList<Qtr3dModel*> &Qtr3dAssets::models() const
{
    return mModels;
}

//-------------------------------------------------------------------------------------------------
const QList<Qtr3dPointCloud*> &Qtr3dAssets::pointClouds() const
{
    return mPointClouds;
}

//-------------------------------------------------------------------------------------------------
Qtr3dEnvironment &Qtr3dAssets::environment()
{
    return mEnvironment;
}

//-------------------------------------------------------------------------------------------------
Qtr3dFpsLoop &Qtr3dAssets::loop()
{
    if (!mLoop) {
        mLoop = new Qtr3dFpsLoop(this);
        connect(mLoop, &Qtr3dFpsLoop::step, &space(), &Qtr3dAbstractSpace::process, Qt::UniqueConnection);
    }
    return *mLoop;
}

//-------------------------------------------------------------------------------------------------
Qtr3dAbstractSpace &Qtr3dAssets::space()
{
    if (!mSpace) {
        mSpace = new Qtr3dAbstractSpace(this);
        connect(&loop(), &Qtr3dFpsLoop::step, mSpace, &Qtr3dAbstractSpace::process, Qt::UniqueConnection);
    }
    return *mSpace;
}

//-------------------------------------------------------------------------------------------------
int Qtr3dAssets::totalModelCount() const
{
    return mModels.count();
}

//-------------------------------------------------------------------------------------------------
int Qtr3dAssets::totalMeshCount() const
{
    int ret = mMeshes.count();
    for (auto *model: mModels)
        ret += model->meshes().count();
    return ret;
}

//-------------------------------------------------------------------------------------------------
int Qtr3dAssets::totalVerticesCount() const
{
    int ret = 0;
    for (const auto *model: mModels) {
        for (const auto *mesh: model->meshes()) {
            ret += mesh->verticesCount();
        }
    }
    for (const auto *mesh: mMeshes) {
        ret += mesh->verticesCount();
    }
    return ret;
}

//-------------------------------------------------------------------------------------------------
Qtr3dMesh *Qtr3dAssets::registerMesh(Qtr3dMesh *mesh)
{
    Q_ASSERT(mesh);
    Q_ASSERT(!mMeshes.contains(mesh));
    mMeshes << mesh;
    connect(mesh, &Qtr3dMesh::destroyed, this, [this](QObject *obj){
       mMeshes.removeOne((Qtr3dMesh*)obj);
    });
    return mesh;
}

//-------------------------------------------------------------------------------------------------
Qtr3dModel *Qtr3dAssets::registerModel(Qtr3dModel *model)
{
    Q_ASSERT(model);
    Q_ASSERT(!mModels.contains(model));
    mModels << model;
    connect(model, &Qtr3dModel::destroyed, this, [this](QObject *obj){
       mModels.removeOne((Qtr3dModel*)obj);
    });
    return model;
}

//-------------------------------------------------------------------------------------------------
Qtr3dPointCloud *Qtr3dAssets::registerPointCloud(Qtr3dPointCloud *cloud)
{
    Q_ASSERT(cloud);
    Q_ASSERT(!mPointClouds.contains(cloud));
    mPointClouds << cloud;
    connect(cloud, &Qtr3dPointCloud::destroyed, this, [this](QObject *obj){
       mPointClouds.removeOne((Qtr3dPointCloud*)obj);
    });
    return cloud;
}
