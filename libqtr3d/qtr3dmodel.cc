
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
    Node *n = new Node();
    if (mesh)
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
QVector3D Qtr3dModel::minValues() const
{
    QVector3D val = QVector3D(  std::numeric_limits<double>::max(),
                       std::numeric_limits<double>::max(),
                       std::numeric_limits<double>::max() );

    for (auto *m: mMeshes) {
        QVector3D meshMin = m->minValues();
        if (val.x() > meshMin.x())
            val.setX(meshMin.x());
        if (val.y() > meshMin.y())
            val.setY(meshMin.y());
        if (val.z() > meshMin.z())
            val.setZ(meshMin.z());
    }

    return val;
}

//-------------------------------------------------------------------------------------------------
QVector3D Qtr3dModel::maxValues() const
{
    QVector3D val = QVector3D(  -std::numeric_limits<double>::max(),
                                -std::numeric_limits<double>::max(),
                                -std::numeric_limits<double>::max() );

    for (auto *m: mMeshes) {
        QVector3D meshMax = m->maxValues();
        if (val.x() < meshMax.x())
            val.setX(meshMax.x());
        if (val.y() < meshMax.y())
            val.setY(meshMax.y());
        if (val.z() < meshMax.z())
            val.setZ(meshMax.z());
    }

    return val;
}

//-------------------------------------------------------------------------------------------------
const Qtr3dMeshes &Qtr3dModel::meshes() const
{
    return mMeshes;
}

//-------------------------------------------------------------------------------------------------
const QList<Qtr3dModel::Node *> &Qtr3dModel::nodes() const
{
    return mNodes;
}

