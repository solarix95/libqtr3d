#ifndef QTR3DFACTORY_H
#define QTR3DFACTORY_H

#include <QVariant>
#include <libqtr3d/loader/qtr3dmodelloader.h>

class Qtr3dMesh;
class Qtr3dTexturedQuad;
class Qtr3dGeometryBufferFactory;
class Qtr3dModel;
class Qtr3dHighmap;

namespace Qtr3d {

bool   modelByFile(Qtr3dModel &model, const QString &filename, Qtr3dModelLoader::Options opts = Qtr3dModelLoader::DefaultOptions);
bool   modelByFileBuffer(Qtr3dModel &model, const QString &filename, const QByteArray &fileBuffer, Qtr3dModelLoader::Options opts = Qtr3dModelLoader::DefaultOptions);
bool   meshByJson(Qtr3dMesh &mesh, const QString &filename);
bool   meshByJson(Qtr3dMesh &mesh, const QVariant &json, const QString &sourceFilenameOrPath = "");
bool   meshByHighmap(Qtr3dMesh &mesh, const QString &highmapImageName, const QString &colorMapImageName, QVector3D scale = {1,1,1});
bool   meshByHighmap(Qtr3dMesh &mesh, const QImage &highmapImage, const QImage &texture, QVector3D scale = {1,1,1});
bool   meshByHighmap(Qtr3dMesh &mesh, const QString &highmapImageName, const QImage &texture, QVector3D scale = {1,1,1});
bool   meshByHighmap(Qtr3dMesh &mesh, const Qtr3dHighmap &highmap, const QImage &texture);
bool   meshByXyzAxis(Qtr3dMesh &mesh, float length = 1); // Debug Purpose
bool   meshByStarsky(Qtr3dMesh &mesh, float radius, int starCount, const QColor &color);
bool   meshByCylinder(Qtr3dMesh &mesh, int sectors, bool topClosed, bool bottomClosed, const QColor &color);
bool   meshBySphere(Qtr3dMesh &mesh, int sectors, const QColor &color);
bool   meshBySphere(Qtr3dMesh &mesh, int sectors, const QImage &colorMap, bool asTexture = false);
bool   meshBySphere(Qtr3dMesh &mesh, int sectors, int rings, float radius, bool renderOutside, const QImage &colorMap, bool asTexture = true);
bool   meshByCycle(Qtr3dMesh &mesh, int sectors, const QColor &color);
bool   meshByParticleTriangle(Qtr3dMesh &mesh, const QColor &color, float radius = 1.0, bool doublesSided = true);
bool   meshByText(Qtr3dMesh &mesh, const QString &text,QFont font, const QColor &frontColor, const QColor &back = Qt::transparent);
bool   meshByTexture(Qtr3dMesh &mesh, const QImage &texture, float width = -1, float height = -1);
bool   meshByChessboard(Qtr3dMesh &mesh, int tilesPerRow, float size, const QColor &color1, const QColor &color2);
bool   normalMeshByMesh(Qtr3dMesh &mesh, const Qtr3dMesh &sourceMesh, float vectorLenght, QColor color = Qt::red);
QImage textureByPath(const QString &localPathOrFile, const QString &textureName);
bool   cloudByFile(Qtr3dPointCloud &cloud, const QString &filename);

bool   appendSphere2Mesh(Qtr3dMesh &mesh, int sectors, const QColor &color, const QMatrix4x4 &transform);
}

#endif // QTR3DFACTORY_H
