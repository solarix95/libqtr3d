#ifndef QTR3D_ASSETS_H
#define QTR3D_ASSETS_H

#include <QObject>
#include <QList>
#include <QOpenGLContext>

#include "qtr3dtypes.h"

class Qtr3dGeometry;
class Qtr3dMesh;
class Qtr3dModel;
class Qtr3dPointCloud;
class Qtr3dGeometryState;
class Qtr3dFpsLoop;
class Qtr3dAbstractSpace;

class Qtr3dAssets : public QOpenGLContext
{
public:
    Qtr3dAssets(QObject *parent = nullptr);

    void        reset();
    void        setLoop(Qtr3dFpsLoop *loop);
    void        setSpace(Qtr3dAbstractSpace *space);

    Qtr3dMesh       *createMesh(bool root = true /* false = part of model */);
    Qtr3dModel      *createModel();
    Qtr3dPointCloud *createPointCloud();

    Qtr3dGeometryState *createState(Qtr3dGeometry *buffer, Qtr3d::LightingType ltype = Qtr3d::DefaultLighting);

    const QList<Qtr3dGeometry*>   &geometries() const;
    const QList<Qtr3dMesh*>       &meshes() const;
    const QList<Qtr3dModel*>      &models() const;
    const QList<Qtr3dPointCloud*> &pointClouds() const;
    Qtr3dEnvironment              &environment();
    Qtr3dFpsLoop                  &loop();
    Qtr3dAbstractSpace            &space();

    // Statistics only
    int totalModelCount()     const;
    int totalMeshCount()      const;
    int totalVerticesCount()  const;

private:
    Qtr3dMesh       *registerMesh(Qtr3dMesh *mesh);
    Qtr3dModel      *registerModel(Qtr3dModel *model);
    Qtr3dPointCloud *registerPointCloud(Qtr3dPointCloud *cloud);
    Qtr3dGeometry   *registerGeometry(Qtr3dGeometry *geometry);

    QList<Qtr3dGeometry*>   mGeometries;

    QList<Qtr3dMesh*>       mMeshes;
    QList<Qtr3dPointCloud*> mPointClouds;
    QList<Qtr3dModel*>      mModels;
    Qtr3dEnvironment        mEnvironment;
    Qtr3dFpsLoop           *mLoop;
    Qtr3dAbstractSpace     *mSpace;
};

#endif // QTR3DASSETS_H
