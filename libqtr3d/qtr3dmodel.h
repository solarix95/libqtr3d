#ifndef QTR3DMODEL_H
#define QTR3DMODEL_H

#include <QObject>
#include "qtr3dmesh.h"
#include "qtr3dgeometrybuffer.h"

class Qtr3dContext;
class Qtr3dModel : public Qtr3dGeometryBuffer
{
    Q_OBJECT
public:
    struct Node {
        Qtr3dMeshes mMeshes;
        Node       *mParent;
        QMatrix4x4  mTranslation;
        QMatrix4x4  translation () const { return mParent ? mParent->translation() * mTranslation : mTranslation; }
    };
    typedef QList<Node*> Nodes;

    explicit Qtr3dModel(Qtr3dContext *context);
    virtual ~Qtr3dModel();

    virtual Node *createNode(Qtr3dMeshes meshes, const QMatrix4x4 &translation, Node *parent = nullptr);
    virtual Node *createNode(Qtr3dMesh  *mesh, const QMatrix4x4 &translation, Node *parent = nullptr);
    virtual Node *createNode(Qtr3dMesh  *mesh, Node *parent = nullptr);
    virtual void  addMesh(Qtr3dMesh *mesh, bool createDefaultNode = false);

    virtual QVector3D minValues() const; // lowest xyz
    virtual QVector3D maxValues() const; // max xyz

    const Qtr3dMeshes &meshes() const;
    const Nodes       &nodes() const;

private:
    Qtr3dMeshes     mMeshes;
    Nodes           mNodes;
};

typedef QList<Qtr3dModel::Node*> Qtr3dModelNodes;

#endif // QTR3DMODEL_H
