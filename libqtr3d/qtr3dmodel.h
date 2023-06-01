#ifndef QTR3DMODEL_H
#define QTR3DMODEL_H

#include <QObject>
#include "qtr3dmesh.h"
#include "qtr3dgeometry.h"

class Qtr3dAssets;
class Qtr3dModelAnimation;
class Qtr3dModel : public Qtr3dGeometry
{
    Q_OBJECT
public:
    struct Node {
        QString     mName;
        Qtr3dMeshes mMeshes;
        Node       *mParent;
        QMatrix4x4  mTranslation;
        QMatrix4x4  translation () const { return mParent ? mParent->translation() * mTranslation : mTranslation; }
    };
    typedef QList<Node*> Nodes;

    explicit Qtr3dModel(Qtr3dAssets *context);
    virtual ~Qtr3dModel();

    virtual Node *createNode(Qtr3dMeshes meshes, const QMatrix4x4 &transform, const QString &name, Node *parent = nullptr);
    virtual Node *createNode(Qtr3dMesh  *mesh, const QMatrix4x4 &transform, const QString &name, Node *parent = nullptr);
    virtual Node *createNode(Qtr3dMesh  *mesh, const QVector3D &translation, const QVector3D &rotation, const QVector3D &scale, const QString &name, Node *parent = nullptr);
    virtual Node *createNode(Qtr3dMesh  *mesh, const QString &name, Node *parent = nullptr);
    virtual void  addMesh(Qtr3dMesh     *mesh, bool createDefaultNode = false);

    virtual QVector3D minValues() const; // lowest xyz
    virtual QVector3D maxValues() const; // max xyz

    const Qtr3dMeshes &meshes() const;
    const Nodes       &nodes() const;

    // Animation Interface
    virtual void        addAnimation(Qtr3dModelAnimation *anim);
    virtual QStringList animations() const;
    virtual Qtr3dModelAnimation *animationByName(const QString &name);

private:
    Qtr3dMeshes                 mMeshes;
    Nodes                       mNodes;
    QList<Qtr3dModelAnimation*> mAnimations;
};

typedef QList<Qtr3dModel::Node*> Qtr3dModelNodes;

#endif // QTR3DMODEL_H
