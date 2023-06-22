
#include "qtr3dassets.h"
#include "qtr3dmesh.h"
#include "qtr3dmodel.h"
#include "qtr3dmodelanimation.h"
#include "qtr3dmodelanimator.h"

//-------------------------------------------------------------------------------------------------
QMatrix4x4  Qtr3dModel::Node::translation() const
{
    return mParent ? mParent->translation() * mTranslation : mTranslation;
}

//-------------------------------------------------------------------------------------------------
Qtr3dModel::Qtr3dModel(Qtr3dAssets *context)
    : Qtr3dGeometry(context)
{
}

//-------------------------------------------------------------------------------------------------
Qtr3dModel::~Qtr3dModel()
{
    mNodes.destroy();
    qDeleteAll(mAnimations);
    // Don't delete Meshes: I'm parent... so they get deleted anyway.
    // qDeleteAll(mMeshes);
}

//-------------------------------------------------------------------------------------------------
Qtr3dModel::Node *Qtr3dModel::createNode(Qtr3dMeshes meshes, const QMatrix4x4 &translation, const QString &name, Node *parent)
{
    Node *n = new Node();
    n->mName   = name;
    n->mMeshes = meshes;
    n->mParent = parent;
    n->mTranslation = translation;

    mNodes.addNode(n);

    // optional: register mesh if the app dont call "addMesh"
    for (auto *mesh: meshes)
        if (!mMeshes.contains(mesh))
            addMesh(mesh,false);
    return n;
}

//-------------------------------------------------------------------------------------------------
Qtr3dModel::Node *Qtr3dModel::createNode(Qtr3dMesh *mesh, const QMatrix4x4 &translation, const QString &name, Node *parent)
{
    Node *n = new Node();
    if (mesh) {
        n->mMeshes = Qtr3dMeshes() << mesh;
        addMesh(mesh,false);
    }
    n->mName   = name;
    n->mParent = parent;
    n->mTranslation = translation;

    mNodes.addNode(n);

    return n;
}

//-------------------------------------------------------------------------------------------------
Qtr3dModel::Node *Qtr3dModel::createNode(Qtr3dMesh *mesh, const QVector3D &translation, const QVector3D &rotation, const QVector3D &scale, const QString &name, Node *parent)
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

    return createNode(mesh,transform,name,parent);
}

//-------------------------------------------------------------------------------------------------
Qtr3dModel::Node *Qtr3dModel::createNode(Qtr3dMesh *mesh, const QString &name, Node *parent)
{
    return createNode(mesh, QMatrix4x4(1,0,0,0,
                                       0,1,0,0,
                                       0,0,1,0,
                                       0,0,0,1), name, parent);
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
        createNode(mesh,"");

    connect(mesh, &Qtr3dMesh::destroyed, this, [this,mesh]() { mMeshes.removeAll(mesh); });
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dModel::addMesh(Qtr3dMesh *mesh, const QString &nodeName)
{
    Q_ASSERT(mesh);
    Q_ASSERT(mesh->context() == this->context());

    if(mMeshes.contains(mesh))
        return false;

    const auto *node = mNodes.nodeByName(nodeName);
    if (!node)
        return false;

    if (!registerMesh(mesh))
        return false;

    const_cast<Node*>(node)->mMeshes << mesh;
    return true;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dModel::addAnimation(Qtr3dModelAnimation *anim)
{
    Q_ASSERT(anim);
    if (anim)
        mAnimations << anim;
}

//-------------------------------------------------------------------------------------------------
QStringList Qtr3dModel::animations() const
{
    QStringList ret;
    for (auto *anim: mAnimations)
        ret << anim->name();
    return ret;
}

//-------------------------------------------------------------------------------------------------
Qtr3dModelAnimation *Qtr3dModel::animationByName(const QString &name)
{
    for (auto *anim: mAnimations)
        if (anim->name() == name)
            return anim;
    return nullptr;
}

//-------------------------------------------------------------------------------------------------
int Qtr3dModel::setupSkeleton(QVector<QMatrix4x4> &skeleton, const Node *node, const Qtr3dMesh *mesh, Qtr3dModelAnimator *animator, const QMatrix4x4 &parentTransform, const QMatrix4x4 &globalTransform)
{
    int updatesBones = 0;
    QMatrix4x4 nodePosition  = node->mTranslation;   // Default-Transform: Static Node Transform

    if (animator)
        animator->transform(node->mName, nodePosition);  // If animated: override static transformation

    nodePosition = parentTransform * nodePosition;   // Tree transformation

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
            if (skeleton.size() < (boneIndex+1))
                skeleton.resize(boneIndex+1);
            skeleton[boneIndex] = globalTransform * nodePosition * bones[boneIndex].offset;
            updatesBones++;
        }
    }

    for (const auto *child: node->mChilds)
        updatesBones += setupSkeleton(skeleton, child,mesh,animator,nodePosition, globalTransform);

    return updatesBones;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dModel::registerMesh(Qtr3dMesh *mesh)
{
    if(mMeshes.contains(mesh))
        return false;

    mesh->setParent(this);
    mesh->setParentBuffer(this);

    mMeshes << mesh;

    connect(mesh, &Qtr3dMesh::destroyed, this, [this,mesh]() { mMeshes.removeAll(mesh); });
    return true;
}

//-------------------------------------------------------------------------------------------------
QVector3D Qtr3dModel::minValues() const
{
    QVector3D val = QVector3D(  std::numeric_limits<double>::max(),
                       std::numeric_limits<double>::max(),
                       std::numeric_limits<double>::max() );

    for (auto *node: mNodes.mNodes) {
        for (auto *m: node->mMeshes) {
            QVector3D meshMin = m->minValues()*node->translation();
            if (val.x() > meshMin.x())
                val.setX(meshMin.x());
            if (val.y() > meshMin.y())
                val.setY(meshMin.y());
            if (val.z() > meshMin.z())
                val.setZ(meshMin.z());
        }
    }

    return val;
}

//-------------------------------------------------------------------------------------------------
QVector3D Qtr3dModel::maxValues() const
{
    QVector3D val = QVector3D(  -std::numeric_limits<double>::max(),
                                -std::numeric_limits<double>::max(),
                                -std::numeric_limits<double>::max() );

    for (auto *node: mNodes.mNodes) {
        for (auto *m: node->mMeshes) {
            QVector3D meshMax = m->maxValues()*node->translation();
            if (val.x() < meshMax.x())
                val.setX(meshMax.x());
            if (val.y() < meshMax.y())
                val.setY(meshMax.y());
            if (val.z() < meshMax.z())
                val.setZ(meshMax.z());
        }
    }

    return val;
}

//-------------------------------------------------------------------------------------------------
const Qtr3dMeshes &Qtr3dModel::meshes() const
{
    return mMeshes;
}

//-------------------------------------------------------------------------------------------------
const Qtr3dModel::Nodes &Qtr3dModel::nodes() const
{
    return mNodes;
}

