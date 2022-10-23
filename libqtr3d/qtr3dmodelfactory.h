#ifndef QTR3DMODELFACTORY_H
#define QTR3DMODELFACTORY_H

#include <QVariant>

class Qtr3dVertexMesh;
class Qtr3dTexturedQuad;

namespace Qtr3dModelFactory {

bool meshByFile(Qtr3dVertexMesh &mesh, const QString &filename);
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

}

#endif // QTR3DMODELFACTORY_H
