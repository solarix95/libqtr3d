#ifndef QTR3DMODEL_H
#define QTR3DMODEL_H

#include <QObject>
#include "qtr3dgeometrybuffer.h"

class Qtr3dTextureFactory;
class Qtr3dMesh;
class Qtr3dContext;

class Qtr3dModel : public Qtr3dGeometryBuffer
{
    Q_OBJECT
public:
    struct Node {
        Qtr3dMesh  *mMesh;
        Node       *mParent;
        QMatrix4x4  mTranslation;
        QMatrix4x4  translation () const { return mParent ? mParent->translation() * mTranslation : mTranslation; }
    };

    explicit Qtr3dModel(Qtr3dContext *context);
    virtual ~Qtr3dModel();

    virtual Node *createNode(Qtr3dMesh *mesh, const QMatrix4x4 &translation, Node *parent = nullptr);
    virtual Node *createNode(Qtr3dMesh *mesh, Node *parent = nullptr);
    virtual void  addMesh(Qtr3dMesh *mesh, bool createDefaultNode = false);

    QVector3D center() const;
    double    radius() const;

    int         meshCount() const;
    Qtr3dMesh  *mesh(int index) const;

    const QList<Node*> &nodes() const;

private:
    QList<Qtr3dMesh*>     mMeshes;
    QList<Node*>          mNodes;
};

typedef QList<Qtr3dModel::Node*> Qtr3dModelNodes;

#endif // QTR3DMODEL_H
