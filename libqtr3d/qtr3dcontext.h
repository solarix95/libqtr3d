#ifndef QTR3DCONTEXT_H
#define QTR3DCONTEXT_H

#include <QObject>
#include <QList>
#include <QOpenGLContext>

#include "qtr3dtypes.h"

class Qtr3dGeometryBuffer;
class Qtr3dMesh;
class Qtr3dModel;
class Qtr3dGeometryBufferState;
class Qtr3dFpsLoop;
class QtrPhSpace;

class Qtr3dContext : public QOpenGLContext
{
public:
    Qtr3dContext(QObject *parent = nullptr);

    Qtr3dMesh  *createMesh(bool root = true);
    Qtr3dModel *createModel();
    void        setLoop(Qtr3dFpsLoop *loop);
    void        setSpace(QtrPhSpace *space);

    Qtr3dGeometryBufferState *createState(Qtr3dGeometryBuffer *buffer, Qtr3d::LightingType ltype = Qtr3d::DefaultLighting);

    const QList<Qtr3dMesh*>  &meshes() const;
    const QList<Qtr3dModel*> &models() const;
    Qtr3dEnvironment         &environment();
    Qtr3dFpsLoop             &loop();
    QtrPhSpace               &space();

private:
    Qtr3dMesh  *registerMesh(Qtr3dMesh *mesh);
    Qtr3dModel *registerModel(Qtr3dModel *model);

    QList<Qtr3dMesh*>  mRootMeshes;
    QList<Qtr3dModel*> mModels;
    Qtr3dEnvironment   mEnvironment;
    Qtr3dFpsLoop      *mLoop;
    QtrPhSpace        *mSpace;
};

#endif // QTR3DCONTEXT_H
