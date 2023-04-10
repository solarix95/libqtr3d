
#include "qtr3dassets.h"
#include "qtr3dmesh.h"
#include "qtr3dmodel.h"

//-------------------------------------------------------------------------------------------------
Qtr3dModel::Qtr3dModel(Qtr3dAssets *context)
    : Qtr3dGeometry(context)
{
}

//-------------------------------------------------------------------------------------------------
Qtr3dModel::~Qtr3dModel()
{
    qDeleteAll(mNodes);

    // Don't delete Meshes: I'm parent... so they get deleted anyway.
    // qDeleteAll(mMeshes);
}

//-------------------------------------------------------------------------------------------------
Qtr3dModel::Node *Qtr3dModel::createNode(Qtr3dMeshes meshes, const QMatrix4x4 &translation, Node *parent)
{
    Node *n = new Node();
    n->mMeshes = meshes;
    n->mParent = parent;
    n->mTranslation = translation;

    mNodes << n;

    // optional: register mesh if the app dont call "addMesh"
    for (auto *mesh: meshes)
        if (!mMeshes.contains(mesh))
            addMesh(mesh,false);
    return n;
}

//-------------------------------------------------------------------------------------------------
Qtr3dModel::Node *Qtr3dModel::createNode(Qtr3dMesh *mesh, const QMatrix4x4 &translation, Node *parent)
{
    Node *n = new Node();
    if (mesh) {
        n->mMeshes = Qtr3dMeshes() << mesh;
        addMesh(mesh,false);
    }
    n->mParent = parent;
    n->mTranslation = translation;

    mNodes << n;

    return n;
}

//-------------------------------------------------------------------------------------------------
Qtr3dModel::Node *Qtr3dModel::createNode(Qtr3dMesh *mesh, const QVector3D &translation, const QVector3D &rotation, const QVector3D &scale, Node *parent)
{
    QMatrix4x4 transform(1,0,0,0,
                         0,1,0,0,
                         0,0,1,0,
                         0,0,0,1);
    transform.translate(translation);
    transform.rotate(rotation.x(),{1,0,0});
    transform.rotate(rotation.y(),{0,1,0});
    transform.rotate(rotation.z(),{0,0,1});
    transform.scale(scale);

    return createNode(mesh,transform,parent);
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

    if(mMeshes.contains(mesh))
        return;

    mesh->setParent(this);
    mesh->setParentBuffer(this);

    mMeshes << mesh;

    if (createDefaultNode)
        createNode(mesh);

    connect(mesh, &Qtr3dMesh::destroyed, this, [this,mesh]() { mMeshes.removeAll(mesh); });
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

