#ifndef QTR3DFACTORY_H
#define QTR3DFACTORY_H

#include <QVariant>
#include <libqtr3d/loader/qtr3dmodelloader.h>

class Qtr3dMesh;
class Qtr3dTexturedQuad;
class Qtr3dGeometryBufferFactory;
class Qtr3dModel;

namespace Qtr3d {

bool modelByFile(Qtr3dModel &model, const QString &filename, Qtr3dModelLoader::Options opts = Qtr3dModelLoader::DefaultOptions);
bool meshByJson(Qtr3dMesh &mesh, const QString &filename);
bool meshByJson(Qtr3dMesh &mesh, const QVariant &json);
bool meshByHighmap(Qtr3dMesh &mesh, const QString &highmapImageName, const QString &colorMapImageName, QVector3D scale = {1,1,1});
bool meshByHighmap(Qtr3dMesh &mesh, const QString &highmapImageName, const QImage &texture, QVector3D scale = {1,1,1});
bool meshByXyzAxis(Qtr3dMesh &mesh); // Debug Purpose
bool meshByStarsky(Qtr3dMesh &mesh, float radius, int starCount, const QColor &color);
bool meshByCylinder(Qtr3dMesh &mesh, int sectors, bool topClosed, bool bottomClosed, const QColor &color);
bool meshBySphere(Qtr3dMesh &mesh, int sectors, const QColor &color);
bool meshBySphere(Qtr3dMesh &mesh, int sectors, const QImage &colorMap);
bool meshByCycle(Qtr3dMesh &mesh, int sectors, const QColor &color);


bool normalMeshByMesh(Qtr3dMesh &mesh, const Qtr3dMesh &sourceMesh, float vectorLenght, QColor color = Qt::red);
}

#endif // QTR3DFACTORY_H