#include <math.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QFileInfo>
#include <QImage>
#include <QDir>
#include <QFont>
#include <QFontMetrics>
#include <QPainter>
#include <QVariant>
#include <QVariantMap>
#include <QVariantList>
#include "qtr3dfactory.h"
#include "qtr3dmesh.h"
#include "loader/qtr3dobjloader.h"
#include "loader/qtr3dstlloader.h"
#include "loader/qtr3dplyloader.h"
#include "loader/qtr3d3dsloader.h"
#include "loader/qtr3dglbloader.h"
#include "utils/qtr3dutils.h"

#ifdef WITH_LIBASSIMP
#include "loader/qtr3dassimploader.h"
#endif

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
bool Qtr3d::meshByJson(Qtr3dMesh &mesh, const QString &filename)
{
    return meshByJson(mesh, sLoadJson(filename), filename);
}

//-------------------------------------------------------------------------------------------------
bool Qtr3d::meshByJson(Qtr3dMesh &mesh, const QVariant &json, const QString &sourceFilenameOrPath)
{
    QVariantMap map = json.toMap();
    if (!map.contains("mesh"))
        return false;

    map = map["mesh"].toMap();


    // Mesh Type Processing
    QString      meshTypeString = map.value("type").toString();
    Qtr3d::MeshType meshType = Qtr3d::UnknownMesh;
    if (meshTypeString == "dot")
        meshType = Qtr3d::Dot;
    else if (meshTypeString == "line")
        meshType = Qtr3d::Line;
    else if (meshTypeString == "triangle")
        meshType = Qtr3d::Triangle;
    else if (meshTypeString == "quad")
        meshType = Qtr3d::Quad;

    if (meshType == Qtr3d::UnknownMesh)
        return false;

    // Vertices Processing
    QVariantList vertices = map.value("vertices").toList();
    if (vertices.isEmpty())
        return false;

    mesh.startMesh(meshType);

    QString defaultColorName = map.value("defaultColor").toString();
    if (!defaultColorName.isEmpty())
        mesh.setDefaultColor(QColor(defaultColorName));

    QString faceOrientationName = map.value("faceOrientation").toString();
    mesh.setFaceOrientation(faceOrientationName == "ccw" ? Qtr3d::CounterClockWise : Qtr3d::ClockWise);


    QString textureName = map.value("texture").toString();
    if (!textureName.isEmpty())
        mesh.setTexture(textureByPath(sourceFilenameOrPath,textureName));

    foreach(QVariant vertex, vertices) {

        // [ x , y,  z ]
        QVariantList list = vertex.toList();
        if (list.count() == 3)
            mesh.addVertex({list[0].toFloat(), list[1].toFloat(), list[2].toFloat()});

        // [ x , y,  z, color ]
        if (list.count() == 4)
            mesh.addVertex({list[0].toFloat(), list[1].toFloat(), list[2].toFloat()}, QColor(list[3].toString()));

        // [ x , y,  z, , nx, ny, nz]
        if (list.count() == 6)
            mesh.addVertex({list[0].toFloat(), list[1].toFloat(), list[2].toFloat()}, QVector3D({list[3].toFloat(), list[4].toFloat(), list[5].toFloat()}));

        // [ x , y,  z, , nx, ny, nz, color]
        if (list.count() == 7)
            mesh.addVertex({list[0].toFloat(), list[1].toFloat(), list[2].toFloat()}, QVector3D({list[3].toFloat(), list[4].toFloat(), list[5].toFloat()}), QColor(list[6].toString()));

        // [ x , y,  z, , nx, ny, nz, tu, tv]
        if (list.count() == 8)
            mesh.addVertex({list[0].toFloat(), list[1].toFloat(), list[2].toFloat()}, QVector3D({list[3].toFloat(), list[4].toFloat(), list[5].toFloat()}), list[6].toFloat(), list[7].toFloat());
    }

    mesh.endMesh();
    return true; // TODO: Validation?
}

//-------------------------------------------------------------------------------------------------
bool Qtr3d::meshByStarsky(Qtr3dMesh &mesh, float radius, int starCount, const QColor &color)
{
    mesh.setDefaultColor(color);
    mesh.startMesh(Qtr3d::Dot);

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
bool Qtr3d::meshBySphere(Qtr3dMesh &mesh, int sectors, const QColor &color)
{
    float       radius = 1.0f;
    float       cx = 0.0f;
    float       cy = 0.0f;
    float       cz = 0.0f;
    const float PI = 3.1415;

    mesh.setDefaultColor(color);
    mesh.startMesh(Qtr3d::Triangle);
    mesh.setFaceOrientation(Qtr3d::CounterClockWise);

    /* top */
    QVector3D p(cx,cy,cz + radius);
    mesh.addVertex(p, p.normalized());

    float stepXY = 2*PI/sectors;
    float angleZ  = PI/(sectors-1);

    // Top (hat)
    {
        float r = sin(angleZ) * radius;
        float z = cos(angleZ);

        for (int s=0; s<sectors; s++) {

            float x = sin(stepXY*s);
            float y = cos(stepXY*s);

            QVector3D p(r*x + cx, r*y + cy, radius*z + cz);
            mesh.addVertex(p,
                           p.normalized());

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

            QVector3D p(r*x + cx, r*y + cy, radius*z + cz);
            mesh.addVertex(p,
                           p.normalized());
        }
    }

    /* Button */
    p = QVector3D(cx, cy, cz - radius);
    mesh.addVertex(p,p.normalized());

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
bool Qtr3d::meshByCycle(Qtr3dMesh &mesh, int sectors, const QColor &color)
{
    float       radius = 1.0f;
    QVector3D   center = {0.f, 0.f, 0.f};

    if (sectors < 3)
        return false;

    mesh.startMesh(Qtr3d::Triangle);
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
bool Qtr3d::meshByXyzAxis(Qtr3dMesh &mesh)
{
    mesh.startMesh(Qtr3d::Line);

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
bool Qtr3d::meshByCylinder(Qtr3dMesh &mesh, int sectors, bool topClosed, bool bottomClosed, const QColor &color)
{
    if (sectors < 3)
        return false;

    QVector3D pointer(1,0,0);
    QMatrix4x4 rotation;
    rotation.rotate(360.f/sectors,{0,1,0});

    mesh.startMesh(Qtr3d::Triangle);
    mesh.setDefaultColor(color);
    mesh.setFaceOrientation(Qtr3d::CounterClockWise);
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
bool Qtr3d::meshBySphere(Qtr3dMesh &mesh, int sectors, const QImage &colorMap)
{
    if (sectors < 3 || colorMap.isNull())
        return false;

    float       radius = 1.0f;
    float       cx = 0.0f;
    float       cy = 0.0f;
    float       cz = 0.0f;
    const float PI = 3.1415;

    mesh.startMesh(Qtr3d::Triangle);
    mesh.setFaceOrientation(Qtr3d::CounterClockWise);

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

            QVector3D p(r*x + cx, r*y + cy, radius*z + cz);
            mesh.addVertex(p,
                           p.normalized(),
                           getColor(1-s/(float)sectors,1.f/sectors));

            /*
            mesh.addVertex({r*x + cx, r*y + cy, radius*z + cz},
                           QVector3D(x,y,z).normalized(),
                           getColor(1-s/(float)sectors,1.f/sectors));
           */

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

            QVector3D p(r*x + cx, r*y + cy, radius*z + cz);
            mesh.addVertex(p,
                           p.normalized(),
                           getColor(1-s/(float)sectors,(ring+2.f)/sectors));
            /*
            mesh.addVertex({r*x + cx, r*y + cy, radius*z + cz},
                           QVector3D(x,y,z),
                           getColor(1-s/(float)sectors,(ring+2.f)/sectors));
            */

        }
    }

    /* Button */
    mesh.addVertex({cx, cy, cz - radius},
                   QVector3D(cx, cy, cz - radius).normalized(), getColor(0.5,1));

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
bool Qtr3d::modelByFile(Qtr3dModel &model, const QString &filename, Qtr3dModelLoader::Options opts)
{
#ifdef WITH_LIBASSIMP
    if (Qtr3dAssimpLoader::supportsFile(filename))
        return Qtr3dAssimpLoader::loadFile(model,filename);
#endif
    if (Qtr3dStlLoader::supportsFile(filename))
        return Qtr3dStlLoader::loadFile(model,filename);

    if (Qtr3dPlyLoader::supportsFile(filename))
        return Qtr3dPlyLoader::loadFile(model,filename);

    if (Qtr3dObjLoader::supportsFile(filename))
        return Qtr3dObjLoader::loadFile(model,filename, opts);

    if (Qtr3d3dsLoader::supportsFile(filename))
        return Qtr3d3dsLoader::loadFile(model,filename);

    if (Qtr3dGlbLoader::supportsFile(filename))
        return Qtr3dGlbLoader::loadFile(model,filename);
    return false;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3d::normalMeshByMesh(Qtr3dMesh &mesh, const Qtr3dMesh &sourceMesh, float vectorLenght, QColor color)
{
    mesh.startMesh(Qtr3d::Line);
    mesh.setDefaultColor(color);

    for (int i=0; i<sourceMesh.vertexListCount(); i++) {
        QVector3D pos           = sourceMesh.vertex(i).p.toQVector();
        QVector3D nextNormal    = sourceMesh.vertex(i).n.toQVector();
        if (nextNormal.isNull())
            continue;
        nextNormal = vectorLenght * nextNormal.normalized();
        mesh.addVertex(pos);
        mesh.addVertex(pos+nextNormal);
    }

    mesh.endMesh();
    return true;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3d::meshByHighmap(Qtr3dMesh &mesh, const QString &highmapImageName, const QString &colorMapImageName, QVector3D scale)
{
    QImage highmapImg;
    if (!highmapImg.load(highmapImageName))
        return false;

    if ((highmapImg.width() * highmapImg.height()) <= 0)
        return false;

    QImage textureImg;
    if (!textureImg.load(colorMapImageName))
        return false;

    if ((textureImg.width() * textureImg.height()) <= 0)
        return false;

    int w  = highmapImg.width();
    int h  = highmapImg.height();
    int tw = textureImg.width();
    int th = textureImg.height();

    mesh.startMesh(Qtr3d::Triangle, Qtr3d::ClockWise);

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            float fx = scale.x() * ((-w/2.0f) + x);
            float fz = scale.z() * ((-h/2.0f) + y);
            float fy = scale.y() * (qGray(highmapImg.pixel(x,y))/255.0f);
            QVector3D normal = {0.0f,1.0f,0.0f};
            if ((y > 0) && (x > 0) && (y < (h-1)) && (x < (w-1))) {
                float topHigh    = qGray(highmapImg.pixel(x,y-1))/255.0f;
                float bottomHigh = qGray(highmapImg.pixel(x,y+1))/255.0f;

                float rightHigh  = qGray(highmapImg.pixel(x+1,y))/255.0f;
                float leftHigh   = qGray(highmapImg.pixel(x-1,y))/255.0f;

                QVector3D horiz = { scale.x()*2, scale.y()*(rightHigh-leftHigh), 0};
                QVector3D verti = { 0,           scale.y()*(bottomHigh-topHigh), scale.z()*2};

                normal = QVector3D::crossProduct(verti,horiz).normalized();
            }

            mesh.addVertex({fx,fy,fz},normal, textureImg.pixel((x/(float)w)*tw, (y/(float)h)*th));
        }
    }

    for (int y = 0; y < h-1; y++) {
        for (int x = 0; x < w-1; x++) {
            int topLeftIndex     = ((y+0)*w) + x + 0;
            int topRightIndex    = ((y+0)*w) + x + 1;
            int bottomLeftIndex  = ((y+1)*w) + x + 0;
            int bottomRightIndex = ((y+1)*w) + x + 1;

            // Triangle 1
            mesh.addIndex(topLeftIndex);
            mesh.addIndex(topRightIndex);
            mesh.addIndex(bottomLeftIndex);

            // Triangle 2
            mesh.addIndex(topRightIndex);
            mesh.addIndex(bottomRightIndex);
            mesh.addIndex(bottomLeftIndex);
        }
    }

    mesh.endMesh();
    return true;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3d::meshByHighmap(Qtr3dMesh &mesh, const QString &highmapImageName, const QImage &texture, QVector3D scale)
{
    QImage highmapImg;
    if (!highmapImg.load(highmapImageName))
        return false;

    if ((highmapImg.width() * highmapImg.height()) <= 0)
        return false;

    if (texture.isNull())
        return false;

    if ((texture.width() * texture.height()) <= 0)
        return false;

    int w  = highmapImg.width();
    int h  = highmapImg.height();

    mesh.startMesh(Qtr3d::Triangle, Qtr3d::ClockWise);
    mesh.setTexture(texture.mirrored());

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            float fx = scale.x() * ((-w/2.0f) + x);
            float fz = scale.z() * ((-h/2.0f) + y);
            float fy = scale.y() * (qGray(highmapImg.pixel(x,y))/255.0f);
            QVector3D normal = {0.0f,1.0f,0.0f};
            if ((y > 0) && (x > 0) && (y < (h-1)) && (x < (w-1))) {
                float topHigh    = qGray(highmapImg.pixel(x,y-1))/255.0f;
                float bottomHigh = qGray(highmapImg.pixel(x,y+1))/255.0f;

                float rightHigh  = qGray(highmapImg.pixel(x+1,y))/255.0f;
                float leftHigh   = qGray(highmapImg.pixel(x-1,y))/255.0f;

                QVector3D horiz = { scale.x()*2, scale.y()*(rightHigh-leftHigh), 0};
                QVector3D verti = { 0,           scale.y()*(bottomHigh-topHigh), scale.z()*2};

                normal = QVector3D::crossProduct(verti,horiz).normalized();
            }

            // mesh.addVertex({fx,fy,fz},normal, y/(float)h, x/(float)w);
            mesh.addVertex({fx,fy,fz},normal, x/(float)w, y/(float)h);
        }
    }

    for (int y = 0; y < h-1; y++) {
        for (int x = 0; x < w-1; x++) {
            int topLeftIndex     = ((y+0)*w) + x + 0;
            int topRightIndex    = ((y+0)*w) + x + 1;
            int bottomLeftIndex  = ((y+1)*w) + x + 0;
            int bottomRightIndex = ((y+1)*w) + x + 1;

            // Triangle 1
            mesh.addIndex(topLeftIndex);
            mesh.addIndex(topRightIndex);
            mesh.addIndex(bottomLeftIndex);

            // Triangle 2
            mesh.addIndex(topRightIndex);
            mesh.addIndex(bottomRightIndex);
            mesh.addIndex(bottomLeftIndex);
        }
    }

    mesh.endMesh();
    return true;

}

//-------------------------------------------------------------------------------------------------
bool Qtr3d::meshByText(Qtr3dMesh &mesh, const QString &text, QFont font, const QColor &frontColor, const QColor &back)
{
   if (text.isEmpty())
       return false;

    QFontMetrics fm(font);
    int height = fm.height();

#if QT_VERSION < QT_VERSION_CHECK(5, 11, 0)
    int width  = fm.width(text);
#else
    int width  = fm.horizontalAdvance(text);
#endif

    if (width <= 0)
        return false;

    QImage img(QSize(width,height),QImage::Format_ARGB32);
    img.fill(back);
    QPainter p(&img);
    p.setFont(font);
    p.setPen(frontColor);
    p.setBrush(frontColor);
    p.drawText(QRect(0,0,width,height),Qt::AlignCenter,text);

    mesh.startMesh(Qtr3d::Quad);
    float normWidth = width/float(height);

    mesh.addVertex(QVector3D(-normWidth/2, -0.5, 0.0),{0,1,0},0,0);
    mesh.addVertex(QVector3D(-normWidth/2,  0.5, 0.0),{0,1,0},0,1);
    mesh.addVertex(QVector3D(normWidth/2,   0.5, 0.0),{0,1,0},1,1);
    mesh.addVertex(QVector3D(normWidth/2,  -0.5, 0.0),{0,1,0},1,0);

    mesh.setTexture(img);
    mesh.endMesh(true);

    return true;
}

//-------------------------------------------------------------------------------------------------
QImage Qtr3d::textureByPath(const QString &localPathOrFile, const QString &textureName)
/*
   Loading a texture (local PNG/JPG-File) from a "base-file" (like json, or OBJ, ...)
*/
{
    /*
       Variants:

         * /tmp/          + textureName
         * /tmp           + textureName
         * /tmp/test.json + textureName
         * :/test.json    + textureName
    */

    QImage ret;

    const QChar sep = QDir::separator();

    // "/tmp" + "texture.png"
    ret = QImage(localPathOrFile + sep + textureName);
    if (!ret.isNull())
        return ret;

    // ":/test.json" + "texture.png"
    int pos = localPathOrFile.lastIndexOf("/");
    if (pos >= 0) {
        QString textureFileName = localPathOrFile.mid(0,pos+1) + textureName;
        ret = QImage(textureFileName);
        if (!ret.isNull())
            return ret;

    }
    return ret;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3d::meshByTexture(Qtr3dMesh &mesh, const QImage &texture, float width, float height)
{
    if (texture.isNull())
        return false;

    if (width <= 0)
        width = texture.width();
    if (height <= 0)
        height = width * texture.height()/float(texture.width());

    float x = width/2;
    float z = height/2;
    mesh.startMesh(Qtr3d::Quad);
    mesh.setTexture(texture);
    mesh.addQuad({-x,0,z},{-x,0,-z},{x,0,-z}, {x,0,z},{1,1,0});
    mesh.endMesh(true);

    return true;
}
