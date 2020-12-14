#include <QFile>
#include <QTextStream>
#include "qtr3dobjloader.h"

//-------------------------------------------------------------------------------------------------
bool Qtr3dObjLoader::supportsFile(const QString &filename)
{
    return filename.toLower().endsWith(".obj");
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dObjLoader::loadFile(Qtr3dVertexMesh &mesh, const QString &filename)
{
    Qtr3dObjLoader loader;
    return loader.loadMesh(mesh,filename);
}

//-------------------------------------------------------------------------------------------------
Qtr3dObjLoader::Qtr3dObjLoader()
 : mMesh(nullptr)
{
}

//-------------------------------------------------------------------------------------------------
Qtr3dObjLoader::~Qtr3dObjLoader()
{

}

//-------------------------------------------------------------------------------------------------
bool Qtr3dObjLoader::loadMesh(Qtr3dVertexMesh &mesh, const QString &filename)
{
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly))
        return false;
    QTextStream in(&f);
    mMesh = &mesh;

    QString     line;
    QString     cmd;
    QStringList parts;
    mMesh->startMesh(Qtr3dVertexMesh::Triangle);
    while (!in.atEnd())
    {
        line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith("#"))
            continue;

        // process https://en.wikipedia.org/wiki/Wavefront_.obj_file
        parts = line.split(" ");
        if (parts.isEmpty())
            continue;
        cmd = parts.takeFirst();
        if (cmd == "v")
            processVertex(parts);
        else if (cmd == "vn")
            processNormal(parts);
        else if (cmd == "f")
            processFace(parts);
        else if (cmd == "s")
            processSmoothshading(parts);

    }
    mMesh->endMesh();
    mMesh = nullptr;

    return true;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dObjLoader::processObject(const QStringList &)
{
}

//-------------------------------------------------------------------------------------------------
void Qtr3dObjLoader::processVertex(const QStringList &args)
{
    if (args.count() < 3)
        return;

    float x = args.at(0).toFloat();
    float y = args.at(1).toFloat();
    float z = args.at(2).toFloat();

    if (args.count() < 6) {
        mMesh->addVertex(QVector3D(x,y,z));
        return;
    }

    // Vertex extradata -> Color, 0..1
    int colorStartIndex = args.count() == 6 ? 3:4; // "x y z r g b" OR "x y z [w] r g b"

    float r = args.at(colorStartIndex+0).toFloat();
    float g = args.at(colorStartIndex+1).toFloat();
    float b = args.at(colorStartIndex+2).toFloat();
    mMesh->addVertex(QVector3D(x,y,z),QColor(r*255, g*255, b*255));
}

//-------------------------------------------------------------------------------------------------
void Qtr3dObjLoader::processNormal(const QStringList &args)
{
    if (args.count() < 3 )
        return;

    float x = args.at(0).toFloat();
    float y = args.at(1).toFloat();
    float z = args.at(2).toFloat();
    mMesh->addNormal(QVector3D(x,y,z));
}

//-------------------------------------------------------------------------------------------------
void Qtr3dObjLoader::processFace(const QStringList &args)
{
    if (args.count() < 3 || args.count() > 4) // Triangles or Quads
        return;

    QStringList i1 = args.at(0).split("/");
    QStringList i2 = args.at(1).split("/");
    QStringList i3 = args.at(2).split("/");

    int v1 = i1.at(0).toInt()-1;
    int v2 = i2.at(0).toInt()-1;
    int v3 = i3.at(0).toInt()-1;

    int n1 = i1.count() == 3 ? i1.at(2).toInt()-1 : -1;
    int n2 = i2.count() == 3 ? i2.at(2).toInt()-1 : -1;
    int n3 = i3.count() == 3 ? i3.at(2).toInt()-1 : -1;

    mMesh->addIndex(v1,-1,n1);
    mMesh->addIndex(v2,-1,n2);
    mMesh->addIndex(v3,-1,n3);

    if (args.count() == 4) {

        QStringList i4 = args.at(3).split("/");
        int v4 = i4.at(0).toInt()-1;
        int n4 = i4.count() == 3 ? i4.at(2).toInt()-1 : -1;

        mMesh->addIndex(v1,-1,n1);
        mMesh->addIndex(v3,-1,n3);
        mMesh->addIndex(v4,-1,n4);
    }
}

//-------------------------------------------------------------------------------------------------
void Qtr3dObjLoader::processSmoothshading(const QStringList &args)
{

}
