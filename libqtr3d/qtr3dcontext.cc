#include "qtr3dcontext.h"
#include "qtr3dmesh.h"
#include "qtr3dmodel.h"

//-------------------------------------------------------------------------------------------------
Qtr3dContext::Qtr3dContext(QObject *parent)
 : QOpenGLContext(parent)
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
Qtr3dGeometryBufferState *Qtr3dContext::createState(Qtr3dGeometryBuffer *buffer, Qtr3d::LightingType ltype)
{
    auto *ret = new Qtr3dGeometryBufferState(this, ltype);
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
