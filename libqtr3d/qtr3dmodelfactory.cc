#include <math.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QVariant>
#include <QVariantMap>
#include <QVariantList>
#include "qtr3dmodelfactory.h"
#include "qtr3dvertexmesh.h"
#include "qtr3dtexturedquad.h"
#include "loader/qtr3dobjloader.h"
#include "loader/qtr3dstlloader.h"
#include "loader/qtr3d3dsloader.h"
#include "loader/qtr3dglbloader.h"
#include "utils/qtr3dutils.h"

//-------------------------------------------------------------------------------------------------
static QVariant sLoadJson(const QString &filename) {
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly))
        return QVariant();

    QByteArray commentedJsonString = f.readAll();
    QByteArray jsonString;

    bool inComment = false;
    for (int i=0; i<commentedJsonString.count(); i++) {
        if (commentedJsonString.mid(i,3) == "###") {
            inComment = true;
            continue;
        }
        if (commentedJsonString[i] == '\n')
            inComment = false;
        if (!inComment)
            jsonString += commentedJsonString[i];
    }

    QJsonParseError parserError;
    QVariant ret = QJsonDocument::fromJson(jsonString,&parserError).toVariant();
    if (parserError.error != QJsonParseError::NoError)
        qWarning() << "Qtr3dModelFactory: JSON Parser Error" << filename << parserError.errorString();
    return ret;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dModelFactory::meshByJson(Qtr3dVertexMesh &mesh, const QString &filename)
{
    return meshByJson(mesh, sLoadJson(filename));
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dModelFactory::meshByJson(Qtr3dVertexMesh &mesh, const QVariant &json)
{
    QVariantMap map = json.toMap();

    QString      meshType = map.value("type").toString();
    QVariantList vertices = map.value("vertices").toList();
    if (meshType.isEmpty() || vertices.isEmpty())
        return false;

    QString defaultColorName = map.value("defaultColor").toString();
    if (!defaultColorName.isEmpty())
        mesh.setDefaultColor(QColor(defaultColorName));
    mesh.startMesh(Qtr3dVertexMesh::Line);
    foreach(QVariant vertex, vertices) {
        QVariantList list = vertex.toList();
        if (list.count() != 3) {
            mesh.reset();
            return false;
        }
        mesh.addVertex({list[0].toFloat(), list[1].toFloat(), list[2].toFloat()});
    }
    mesh.endMesh();
    return true; // TODO: Validation?
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dModelFactory::meshByStarsky(Qtr3dVertexMesh &mesh, float radius, int starCount, const QColor &color)
{
    mesh.setDefaultColor(color);
    mesh.startMesh(Qtr3dVertexMesh::Dot);

    for (int i=0; i<starCount; i++) {
        QVector3D v((float)(-10000 + (Qtr3d::qrand()%20000)),
                    (float)(-10000 + (Qtr3d::qrand()%20000)),
                    (float)(-10000 + (Qtr3d::qrand()%20000)));
        v.normalize();
        v = v * radius;
        mesh.addVertex(v);
    }

    mesh.endMesh();
    return true;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dModelFactory::meshBySphere(Qtr3dVertexMesh &mesh, int sectors, const QColor &color)
{
    float       radius = 1.0f;
    float       cx = 0.0f;
    float       cy = 0.0f;
    float       cz = 0.0f;
    const float PI = 3.1415;

    mesh.setDefaultColor(color);
    mesh.startMesh(Qtr3dVertexMesh::Triangle);

    /* top */
    mesh.addVertex({cx,cy,cz + radius}, QVector3D(0.f, 0.f, 1.f), color);

    float stepXY = 2*PI/sectors;
    float angleZ  = PI/(sectors-1);

    // Top (hat)
    {
        float r = sin(angleZ) * radius;
        float z = cos(angleZ);

        for (int s=0; s<sectors; s++) {

            float x = sin(stepXY*s);
            float y = cos(stepXY*s);

            mesh.addVertex({r*x + cx, r*y + cy, radius*z + cz},
                           QVector3D(x,y,z));

            mesh.addIndex(s+1);
            mesh.addIndex(0); // all polygons to the top center
            mesh.addIndex(s == (sectors-1) ? 1 : s+2);
        }
    }

    for (int ring = 0; ring<sectors-3; ring++) {
        // r = sin(PI * ((ring+2)*stepZ)/2) * radius;
        float r = sin((ring+2)*angleZ) * radius;
        float z = cos((ring+2)*angleZ);

        for (int s=0; s<sectors; s++) {

            float x = sin(stepXY*s);
            float y = cos(stepXY*s);

            mesh.addVertex({r*x + cx, r*y + cy, radius*z + cz},
                           QVector3D(x,y,z));

        }
    }

    /* Button */
    mesh.addVertex({cx, cy, cz - radius},
                   QVector3D(0.f,0.f,-1.f), color);

    for (int ring = 0; ring<sectors-3; ring++) {
        for (int s=0; s<sectors; s++) {

            int first = 1 +(ring*sectors);

            // Oberes Dreieck
            mesh.addIndex(first + s);
            mesh.addIndex(s == sectors - 1 ? (first) : (first + s + 1));
            mesh.addIndex(first + s + sectors);

            // Unteres Dreieck
            mesh.addIndex(first + s);
            mesh.addIndex(first + s + sectors);
            mesh.addIndex(s == 0 ? (first + 2*sectors - 1) : (first + s + sectors - 1));
        }
    }

    // Button
    for (int s=0; s<sectors; s++) {
        int first = 1+ ((sectors-3)*sectors);

        mesh.addIndex(first + s);
        mesh.addIndex(s == sectors - 1 ? (first) : (first + s + 1));
        mesh.addIndex(1 + (sectors-2)*sectors);

    }

    mesh.endMesh();
    return true;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dModelFactory::meshByCycle(Qtr3dVertexMesh &mesh, int sectors, const QColor &color)
{
    float       radius = 1.0f;
    QVector3D   center = {0.f, 0.f, 0.f};

    if (sectors < 3)
        return false;

    mesh.startMesh(Qtr3dVertexMesh::Triangle);
    mesh.setDefaultColor(color);

    QVector3D  pointer = {radius, 0.f, 0.f};
    QVector3D  normal  = {0.f, 1.f, 0.f};
    QMatrix4x4 rotation;
    // rotation.data()[15] = 1;
    rotation.rotate(360.f/sectors,normal);

    mesh.addVertex(center, normal); // add "Center", Vertex 0
    for (int i=0; i<sectors; i++) {
        mesh.addVertex(pointer,normal);
        pointer = pointer * rotation;

        // Triangle...
        mesh.addIndex(i+1);                        // from current
        mesh.addIndex(0);                          // to center..
        mesh.addIndex(i == sectors-1 ? 1 : (i+2)); // to the next OR the first..
    }

    mesh.endMesh();
    return true;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dModelFactory::meshByXyzAxis(Qtr3dVertexMesh &mesh)
{
    mesh.startMesh(Qtr3dVertexMesh::Line);

    // 4 Dots
    mesh.addVertex({0,0,0}, Qt::white);
    mesh.addVertex({1,0,0}, Qt::red);
    mesh.addVertex({0,1,0}, Qt::green);
    mesh.addVertex({0,0,1}, Qt::blue);

    // 3 Lines
    mesh.addIndex(0);
    mesh.addIndex(1);

    mesh.addIndex(0);
    mesh.addIndex(2);

    mesh.addIndex(0);
    mesh.addIndex(3);

    mesh.endMesh();

    return true;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dModelFactory::meshByCylinder(Qtr3dVertexMesh &mesh, int sectors, bool topClosed, bool bottomClosed, const QColor &color)
{
    if (sectors < 3)
        return false;

    QVector3D pointer(1,0,0);
    QMatrix4x4 rotation;
    rotation.rotate(360.f/sectors,{0,1,0});

    mesh.startMesh(Qtr3dVertexMesh::Triangle);
    mesh.setDefaultColor(color);
    for (int i=0; i<sectors; i++) {
        mesh.addVertex(pointer + QVector3D(0, 0.5,0),pointer); // Top Cycle
        mesh.addVertex(pointer + QVector3D(0,-0.5,0),pointer); // Bottom Cycle

        int currentTop    = (i*2)+0;
        int currentBottom = (i*2)+1;
        int nextTop       = i == sectors-1 ? 0 : (i+1)*2+0;
        int nextBottom    = i == sectors-1 ? 1 : (i+1)*2+1;

        // add triangle 1
        mesh.addIndex(currentBottom);
        mesh.addIndex(currentTop);
        mesh.addIndex(nextTop);

        // add triangle 2
        mesh.addIndex(nextTop);
        mesh.addIndex(nextBottom);
        mesh.addIndex(currentBottom);

        // mesh.addIndex(i == sectors - 1 ? 0 : (i+1)*2); // to next top

        pointer = pointer * rotation;
    }

    mesh.endMesh();
    return true;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dModelFactory::meshBySphere(Qtr3dVertexMesh &mesh, int sectors, const QImage &colorMap)
{
    if (sectors < 3 || colorMap.isNull())
        return false;

    float       radius = 1.0f;
    float       cx = 0.0f;
    float       cy = 0.0f;
    float       cz = 0.0f;
    const float PI = 3.1415;

    mesh.startMesh(Qtr3dVertexMesh::Triangle);

    auto getColor = [&](float xProc, float yProc) {
        return colorMap.pixel(xProc * (colorMap.width()-1),
                              yProc * (colorMap.height()-1));
    };

    /* top */
    mesh.addVertex({cx,cy,cz + radius}, QVector3D(0.f, 0.f, 1.f), getColor(0.5,0));

    float stepXY = 2*PI/sectors;
    float angleZ  = PI/(sectors-1);

    // Top (hat)
    {
        float r = sin(angleZ) * radius;
        float z = cos(angleZ);

        for (int s=0; s<sectors; s++) {

            float x = sin(stepXY*s);
            float y = cos(stepXY*s);

            mesh.addVertex({r*x + cx, r*y + cy, radius*z + cz},
                           QVector3D(x,y,z),
                           getColor(1-s/(float)sectors,1.f/sectors));

            mesh.addIndex(s+1);
            mesh.addIndex(0); // all polygons to the top center
            mesh.addIndex(s == (sectors-1) ? 1 : s+2);
        }
    }

    for (int ring = 0; ring<sectors-3; ring++) {
        // r = sin(PI * ((ring+2)*stepZ)/2) * radius;
        float r = sin((ring+2)*angleZ) * radius;
        float z = cos((ring+2)*angleZ);

        for (int s=0; s<sectors; s++) {

            float x = sin(stepXY*s);
            float y = cos(stepXY*s);

            mesh.addVertex({r*x + cx, r*y + cy, radius*z + cz},
                           QVector3D(x,y,z),
                           getColor(1-s/(float)sectors,(ring+2.f)/sectors));

        }
    }

    /* Button */
    mesh.addVertex({cx, cy, cz - radius},
                   QVector3D(0.f,0.f,-1.f), getColor(0.5,1));

    for (int ring = 0; ring<sectors-3; ring++) {
        for (int s=0; s<sectors; s++) {

            int first = 1 +(ring*sectors);

            // Oberes Dreieck
            mesh.addIndex(first + s);
            mesh.addIndex(s == sectors - 1 ? (first) : (first + s + 1));
            mesh.addIndex(first + s + sectors);

            // Unteres Dreieck
            mesh.addIndex(first + s);
            mesh.addIndex(first + s + sectors);
            mesh.addIndex(s == 0 ? (first + 2*sectors - 1) : (first + s + sectors - 1));
        }
    }

    // Button
    for (int s=0; s<sectors; s++) {
        int first = 1+ ((sectors-3)*sectors);

        mesh.addIndex(first + s);
        mesh.addIndex(s == sectors - 1 ? (first) : (first + s + 1));
        mesh.addIndex(1 + (sectors-2)*sectors);

    }

    mesh.endMesh();
    return true;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dModelFactory::quadsByJson(Qtr3dTexturedQuad &mesh, const QString &filename)
{
    return quadsByJson(mesh, sLoadJson(filename));
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dModelFactory::quadsByJson(Qtr3dTexturedQuad &mesh, const QVariant &json)
{
    QVariantMap map = json.toMap();

    QString      textureName = map.value("texture").toString();
    QVariantList quads = map.value("quads").toList();
    if (textureName.isEmpty() || quads.isEmpty())
        return false;

    mesh.startMesh(textureName);

    foreach(QVariant quad, quads) {
        QVariantList vertices = quad.toList();
        if (vertices.count() != 4) {
            mesh.reset();
            return false;
        }
        auto toVector = [](const QVariant &v) {
                QVariantList coords = v.toList();
                if (coords.count() != 3)
                    return QVector3D();
                return QVector3D(coords[0].toFloat(), coords[1].toFloat(), coords[2].toFloat());
        };
        mesh.addQuad(toVector(vertices[0]), toVector(vertices[1]),
                     toVector(vertices[2]), toVector(vertices[3]));
    }

    mesh.endMesh();
    return true; // TODO: Validation?
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dModelFactory::modelByFile(Qtr3dModel &model, const QString &filename, Qtr3dGeometryBufferFactory &factory, Qtr3dModelLoader::Options opts)
{
    if (Qtr3dStlLoader::supportsFile(filename))
        return Qtr3dStlLoader::loadFile(model,filename, factory);

    if (Qtr3dObjLoader::supportsFile(filename))
        return Qtr3dObjLoader::loadFile(model,filename, factory, opts);

    if (Qtr3d3dsLoader::supportsFile(filename))
        return Qtr3d3dsLoader::loadFile(model,filename, factory);

    if (Qtr3dGlbLoader::supportsFile(filename))
        return Qtr3dGlbLoader::loadFile(model,filename, factory);
    return false;
}
