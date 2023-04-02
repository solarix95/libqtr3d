
#include "qtr3dcontext.h"
#include "qtr3dmesh.h"
#include "qtr3dmodel.h"
#include "physics/qtr3dfpsloop.h"
#include "physics/qtr3dabstractspace.h"

//-------------------------------------------------------------------------------------------------
Qtr3dContext::Qtr3dContext(QObject *parent)
 : QOpenGLContext(parent)
 , mLoop(nullptr)
 , mSpace(nullptr)
{
}

//-------------------------------------------------------------------------------------------------
Qtr3dMesh *Qtr3dContext::createMesh(bool root)
{
    auto *ret = new Qtr3dMesh(this);
    return root ? registerMesh(ret) : ret;
}

//-------------------------------------------------------------------------------------------------
Qtr3dModel *Qtr3dContext::createModel()
{
    return registerModel(new Qtr3dModel(this));
}

//-------------------------------------------------------------------------------------------------
void Qtr3dContext::setLoop(Qtr3dFpsLoop *loop)
{
    Q_ASSERT(loop);
    if (mLoop)
        mLoop->deleteLater();
    mLoop = loop;
    connect(mLoop, &Qtr3dFpsLoop::step, &space(), &Qtr3dAbstractSpace::process, Qt::UniqueConnection);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dContext::setSpace(Qtr3dAbstractSpace *space)
{
    Q_ASSERT(space);
    if (mSpace)
        mSpace->deleteLater();
    mSpace = space;
    connect(&loop(), &Qtr3dFpsLoop::step,mSpace, &Qtr3dAbstractSpace::process, Qt::UniqueConnection);
}

//-------------------------------------------------------------------------------------------------
Qtr3dGeometryState *Qtr3dContext::createState(Qtr3dGeometry *buffer, Qtr3d::LightingType ltype)
{
    auto *ret = new Qtr3dGeometryState(buffer, ltype);
    buffer->registerBufferState(ret);
    return ret;
}

//-------------------------------------------------------------------------------------------------
const QList<Qtr3dMesh*> &Qtr3dContext::meshes() const
{
    return mRootMeshes;
}

//-------------------------------------------------------------------------------------------------
const QList<Qtr3dModel*> &Qtr3dContext::models() const
{
    return mModels;
}

//-------------------------------------------------------------------------------------------------
Qtr3dEnvironment &Qtr3dContext::environment()
{
    return mEnvironment;
}

//-------------------------------------------------------------------------------------------------
Qtr3dFpsLoop &Qtr3dContext::loop()
{
    if (!mLoop) {
        mLoop = new Qtr3dFpsLoop(this);
        connect(mLoop, &Qtr3dFpsLoop::step, &space(), &Qtr3dAbstractSpace::process, Qt::UniqueConnection);
    }
    return *mLoop;
}

//-------------------------------------------------------------------------------------------------
Qtr3dAbstractSpace &Qtr3dContext::space()
{
    if (!mSpace) {
        mSpace = new Qtr3dAbstractSpace(this);
        connect(&loop(), &Qtr3dFpsLoop::step, mSpace, &Qtr3dAbstractSpace::process, Qt::UniqueConnection);
    }
    return *mSpace;
}

//-------------------------------------------------------------------------------------------------
Qtr3dMesh *Qtr3dContext::registerMesh(Qtr3dMesh *mesh)
{
    Q_ASSERT(mesh);
    Q_ASSERT(!mRootMeshes.contains(mesh));
    mRootMeshes << mesh;
    connect(mesh, &Qtr3dMesh::destroyed, this, [this](QObject *obj){
       mRootMeshes.removeOne((Qtr3dMesh*)obj);
    });
    return mesh;
}

//-------------------------------------------------------------------------------------------------
Qtr3dModel *Qtr3dContext::registerModel(Qtr3dModel *model)
{
    Q_ASSERT(model);
    Q_ASSERT(!mModels.contains(model));
    mModels << model;
    connect(model, &Qtr3dModel::destroyed, this, [this](QObject *obj){
       mModels.removeOne((Qtr3dModel*)obj);
    });
    return model;
}
