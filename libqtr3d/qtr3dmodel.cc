
#include "qtr3dcontext.h"
#include "qtr3dmesh.h"
#include "qtr3dmodel.h"

//-------------------------------------------------------------------------------------------------
Qtr3dModel::Qtr3dModel(Qtr3dContext *context)
    : Qtr3dGeometryBuffer(context)
{
}

//-------------------------------------------------------------------------------------------------
Qtr3dModel::~Qtr3dModel()
{
    qDeleteAll(mNodes);
    qDeleteAll(mMeshes);
}

//-------------------------------------------------------------------------------------------------
Qtr3dModel::Node *Qtr3dModel::createNode(Qtr3dMeshes meshes, const QMatrix4x4 &translation, Node *parent)
{
    Node *n = new Node();
    n->mMeshes = meshes;
    n->mParent = parent;
    n->mTranslation = translation;

    mNodes << n;

    return n;
}

//-------------------------------------------------------------------------------------------------
Qtr3dModel::Node *Qtr3dModel::createNode(Qtr3dMesh *mesh, const QMatrix4x4 &translation, Node *parent)
{
    Q_ASSERT(!mesh || mMeshes.contains(mesh));
    Node *n = new Node();
    n->mMeshes = Qtr3dMeshes() << mesh;
    n->mParent = parent;
    n->mTranslation = translation;

    mNodes << n;

    return n;
}

//-------------------------------------------------------------------------------------------------
Qtr3dModel::Node *Qtr3dModel::createNode(Qtr3dMesh *mesh, Node *parent)
{
    return createNode(mesh, QMatrix4x4(1,0,0,0,
                                       0,1,0,0,
                                       0,0,1,0,
                                       0,0,0,1), parent);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dModel::addMesh(Qtr3dMesh *mesh, bool createDefaultNode)
{
    Q_ASSERT(mesh);
    Q_ASSERT(mesh->context() == this->context());
    mesh->setParentBuffer(this);
    mMeshes << mesh;

    if (createDefaultNode)
        createNode(mesh);
}

//-------------------------------------------------------------------------------------------------
QVector3D Qtr3dModel::center() const
{
    return mMeshes.isEmpty() ? QVector3D() : mMeshes.first()->center();
}

//-------------------------------------------------------------------------------------------------
double Qtr3dModel::radius() const
{
    return mMeshes.isEmpty() ? 0 : mMeshes.first()->radius();
}

//-------------------------------------------------------------------------------------------------
int Qtr3dModel::meshCount() const
{
    return mMeshes.count();
}

//-------------------------------------------------------------------------------------------------
Qtr3dMesh *Qtr3dModel::mesh(int index) const
{
    Q_ASSERT(index >= 0);
    Q_ASSERT(index < mMeshes.count());
    return mMeshes[index];
}

//-------------------------------------------------------------------------------------------------
const QList<Qtr3dModel::Node *> &Qtr3dModel::nodes() const
{
    return mNodes;
}

