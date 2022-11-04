#ifndef QTR3DMODELFACTORY_H
#define QTR3DMODELFACTORY_H

#include <QVariant>
#include <libqtr3d/loader/qtr3dmodelloader.h>
class Qtr3dVertexMesh;
class Qtr3dTexturedQuad;
class Qtr3dGeometryBufferFactory;
class Qtr3dModel;

namespace Qtr3dModelFactory {

bool meshByFile(Qtr3dVertexMesh &mesh, const QString &filename);
bool modelByFile(Qtr3dModel &model, const QString &filename, Qtr3dGeometryBufferFactory &factory, Qtr3dModelLoader::Options opts = Qtr3dModelLoader::DefaultOptions);
bool meshByJson(Qtr3dVertexMesh &mesh, const QString &filename);
bool meshByJson(Qtr3dVertexMesh &mesh, const QVariant &json);

bool meshByXyzAxis(Qtr3dVertexMesh &mesh); // Debug Purpose
bool meshByStarsky(Qtr3dVertexMesh &mesh, float radius, int starCount, const QColor &color);
bool meshByCylinder(Qtr3dVertexMesh &mesh, int sectors, bool topClosed, bool bottomClosed, const QColor &color);
bool meshBySphere(Qtr3dVertexMesh &mesh, int sectors, const QColor &color);
bool meshBySphere(Qtr3dVertexMesh &mesh, int sectors, const QImage &colorMap);
bool meshByCycle(Qtr3dVertexMesh &mesh, int sectors, const QColor &color);

bool quadsByJson(Qtr3dTexturedQuad &mesh, const QString &filename);
bool quadsByJson(Qtr3dTexturedQuad &mesh, const QVariant &json);

bool normalMeshByMesh(Qtr3dVertexMesh &mesh, const Qtr3dVertexMesh &sourceMesh, float vectorLenght, QColor color = Qt::red);
}

#endif // QTR3DMODELFACTORY_H
