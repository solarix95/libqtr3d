#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>

#include "../qtr3dtexturedmesh.h"
#include "qtr3dobjloader.h"

//-------------------------------------------------------------------------------------------------
bool Qtr3dObjLoader::supportsFile(const QString &filename)
{
    return filename.toLower().endsWith(".obj");
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dObjLoader::loadFile(Qtr3dModel &model, const QString &filename, Qtr3dGeometryBufferFactory &factory, Options opts)
{
    Qtr3dObjLoader loader;
    return loader.loadModel(model,filename, factory, opts);
}

//-------------------------------------------------------------------------------------------------
Qtr3dObjLoader::Qtr3dObjLoader()
    : mMesh(nullptr)
{
}

//-------------------------------------------------------------------------------------------------
Qtr3dObjLoader::~Qtr3dObjLoader() = default;

//-------------------------------------------------------------------------------------------------
bool Qtr3dObjLoader::loadModel(Qtr3dModel &model, const QString &filename, Qtr3dGeometryBufferFactory &factory, Options opts)
{
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly))
        return false;
    QTextStream in(&f);

    QString     line;
    QString     cmd;
    QStringList parts;

    while (!in.atEnd())
    {
        line = in.readLine().trimmed();
        int commentPos = line.indexOf('#');
        if (commentPos >= 0)
            line = line.mid(0,commentPos).trimmed().simplified();

        if (line.isEmpty())
            continue;

        // process https://en.wikipedia.org/wiki/Wavefront_.obj_file
        parts = line.split(" ");
        if (parts.isEmpty())
            continue;
        cmd = parts.takeFirst();
        if (cmd == "o")
            processObject(parts);
        if (cmd == "g")
            processGroup(parts);
        else if (cmd == "v")
            processVertex(parts);
        else if (cmd == "vn")
            processNormal(parts);
        else if (cmd == "vt")
            processTextureCoords(parts);
        else if (cmd == "f")
            processFace(parts);
        else if (cmd == "s")
            processSmoothshading(parts);
        else if (cmd == "mtllib")
            processMaterialLib(filename,parts);

    }

    if (mVertices.isEmpty())
        return false;

    if (opts.testFlag(VertexPoints)) {
        setupVertexDotMesh(model,factory);
        return true;
    }

    if (!mTextureName.isEmpty() && !mTextureCoords.isEmpty())
        setupTexturedMesh(model, factory);
    else
        setupSimpleMesh(model, factory);

    return true;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dObjLoader::processObject(const QStringList &args)
{
    qDebug() << "OBJ, Object: " << args;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dObjLoader::processGroup(const QStringList &args)
{
    qDebug() << "OBJ, Group: " << args;
}


//-------------------------------------------------------------------------------------------------
void Qtr3dObjLoader::processVertex(const QStringList &args)
{
    if (args.count() < 3)
        return;

    float x = args.at(0).toFloat();
    float y = args.at(1).toFloat();
    float z = args.at(2).toFloat();

    mVertices << QVector3D(x,y,z);
    if (args.count() < 6)
        return;
        // mMesh->addVertex(QVector3D(x,y,z));


    // Vertex extradata -> Color, 0..1
    int colorStartIndex = args.count() == 6 ? 3:4; // "x y z r g b" OR "x y z [w] r g b"

    float r = args.at(colorStartIndex+0).toFloat();
    float g = args.at(colorStartIndex+1).toFloat();
    float b = args.at(colorStartIndex+2).toFloat();
    // mMesh->addVertex(QVector3D(x,y,z),QColor(r*255, g*255, b*255));
    mVerticesColors << QColor(r*255, g*255, b*255);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dObjLoader::processNormal(const QStringList &args)
{
    if (args.count() < 3 )
        return;

    float x = args.at(0).toFloat();
    float y = args.at(1).toFloat();
    float z = args.at(2).toFloat();
   //  mMesh->addNormal(QVector3D(x,y,z));
    mNormals << QVector3D(x,y,z);
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

    int t1 = i1.count() >= 2 ? i1.at(1).toInt()-1 : -1;
    int t2 = i2.count() >= 2 ? i2.at(1).toInt()-1 : -1;
    int t3 = i3.count() >= 2 ? i3.at(1).toInt()-1 : -1;

    int n1 = i1.count() == 3 ? i1.at(2).toInt()-1 : -1;
    int n2 = i2.count() == 3 ? i2.at(2).toInt()-1 : -1;
    int n3 = i3.count() == 3 ? i3.at(2).toInt()-1 : -1;

    mFaceVertexIndexes << v1;
    mFaceVertexIndexes << v2;
    mFaceVertexIndexes << v3;

    mFaceTextureIndexes << t1;
    mFaceTextureIndexes << t2;
    mFaceTextureIndexes << t3;

    mFaceNormalsIndexes << n1;
    mFaceNormalsIndexes << n2;
    mFaceNormalsIndexes << n3;


    if (args.count() == 4) {

        QStringList i4 = args.at(3).split("/");
        int v4 = i4.at(0).toInt()-1;
        int t4 = i4.count() >= 2 ? i4.at(1).toInt()-1 : -1;
        int n4 = i4.count() == 3 ? i4.at(2).toInt()-1 : -1;

        mFaceVertexIndexes << v1;
        mFaceVertexIndexes << v3;
        mFaceVertexIndexes << v4;

        mFaceTextureIndexes << t1;
        mFaceTextureIndexes << t3;
        mFaceTextureIndexes << t4;

        mFaceNormalsIndexes << n1;
        mFaceNormalsIndexes << n3;
        mFaceNormalsIndexes << n4;
    }

    if (args.count() > 4)
        qDebug() << "WTF";
}

//-------------------------------------------------------------------------------------------------
void Qtr3dObjLoader::processTextureCoords(const QStringList &args)
{
    if (args.count() != 2)
        return;
    mTextureCoords << QPointF(args[0].toFloat(), args[1].toFloat());
}

//-------------------------------------------------------------------------------------------------
void Qtr3dObjLoader::processSmoothshading(const QStringList &args)
{
}

//-------------------------------------------------------------------------------------------------
void Qtr3dObjLoader::processMaterialLib(const QString &sourcefile, const QStringList &args)
{
    if (args.count() != 1)
        return;

    QString mathlibFile = addPath(sourcefile,args.first());

    if (!QFileInfo::exists(mathlibFile))
        return;

    QFile f(mathlibFile);
    if (!f.open(QIODevice::ReadOnly))
        return;

    QTextStream in(&f);

    QString     line;
    QString     cmd;
    QStringList parts;

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
        if (cmd == "map_Kd")
            processMaterialTexture(mathlibFile,parts);
    }
}

//-------------------------------------------------------------------------------------------------
void Qtr3dObjLoader::processMaterialTexture(const QString &matlibFilename, const QStringList &args)
{
    if (args.count() != 1)
        return;
    QString textureName = addPath(matlibFilename,args.first());
    if (QFileInfo::exists(textureName))
        mTextureName = textureName;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dObjLoader::setupTexturedMesh(Qtr3dModel &model,  Qtr3dGeometryBufferFactory &factory)
{

    Qtr3dTexturedMesh *mesh = factory.createTexturedMesh(mTextureName);

    mesh->startMesh();

    // Faces
    for (int i=0; i<mFaceVertexIndexes.count(); i++) {
        mesh->addVertex(mVertices[mFaceVertexIndexes.at(i)],
                        mTextureCoords[mFaceTextureIndexes.at(i)].x(),
                        mTextureCoords[mFaceTextureIndexes.at(i)].y(),
                        mNormals[mFaceNormalsIndexes.at(i)]);

    }
    mesh->endMesh();
    model.addGeometry(mesh);


    /*

    // Debug, Pseudo-Texture

    auto *mesh = factory.createVertexMesh();
    mesh->startMesh(Qtr3dGeometryBuffer::Triangle);

    QImage img(mTextureName);
    if (img.isNull())
        return;

    img = img.mirrored();

    // Faces
    for (int i=0; i<mFaceVertexIndexes.count(); i++) {
        int x = img.width() * mTextureCoords[mFaceTextureIndexes.at(i)].x();
        int y = img.width() * mTextureCoords[mFaceTextureIndexes.at(i)].y();
        // mesh->addVertex(mVertices[mFaceVertexIndexes.at(i)], QColor(128+ (qrand() % 127),128+ (qrand() % 127),128+ (qrand() % 127)));
        mesh->addVertex(mVertices[mFaceVertexIndexes.at(i)], QColor::fromRgb(img.pixel(x,y)));
    }
    mesh->endMesh();
    model.addGeometry(mesh);

    */
}

//-------------------------------------------------------------------------------------------------
void Qtr3dObjLoader::setupSimpleMesh(Qtr3dModel &model, Qtr3dGeometryBufferFactory &factory)
{
    Qtr3dVertexMesh *mesh = factory.createVertexMesh();
    mesh->setDefaultColor(Qt::white);
    mesh->startMesh(Qtr3dVertexMesh::Triangle);

    bool colored = mVerticesColors.count() == mVertices.count();

    // Vertices
    for (int i=0; i<mVertices.count(); i++) {
        if (colored)
            mesh->addVertex(mVertices.at(i),mVerticesColors.at(i));
        else
            mesh->addVertex(mVertices.at(i));
    }

    for (int i=0; i<mNormals.count(); i++)
        mesh->addNormal(mNormals.at(i));

    // Faces
    for (int i=0; i<mFaceVertexIndexes.count(); i++)
        mesh->addIndex(mFaceVertexIndexes.at(i),-1,mFaceNormalsIndexes.at(i));

    mesh->endMesh();
    model.addGeometry(mesh);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dObjLoader::setupVertexDotMesh(Qtr3dModel &model, Qtr3dGeometryBufferFactory &factory)
{
    Qtr3dVertexMesh *mesh = factory.createVertexMesh();
    mesh->setDefaultColor(Qt::white);
    mesh->startMesh(Qtr3dVertexMesh::Dot);

    bool colored = mVerticesColors.count() == mVertices.count();

    // Vertices
    for (int i=0; i<mVertices.count(); i++) {
        if (colored)
            mesh->addVertex(mVertices.at(i),mVerticesColors.at(i));
        else
            mesh->addVertex(mVertices.at(i));
    }


    mesh->endMesh();
    model.addGeometry(mesh);
}

