#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QImage>
#include <QTextStream>

#include "qtr3dobjloader.h"

//-------------------------------------------------------------------------------------------------
bool Qtr3dObjLoader::supportsFile(const QString &filename)
{
    return filename.toLower().endsWith(".obj");
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dObjLoader::loadFile(Qtr3dModel &model, const QString &filename, Options opts)
{
    Qtr3dObjLoader loader;
    return loader.loadModel(model,filename,opts);
}

//-------------------------------------------------------------------------------------------------
Qtr3dObjLoader::Qtr3dObjLoader()
    : mMesh(nullptr)
{
}

//-------------------------------------------------------------------------------------------------
Qtr3dObjLoader::~Qtr3dObjLoader() = default;

//-------------------------------------------------------------------------------------------------
bool Qtr3dObjLoader::loadModel(Qtr3dModel &model, const QString &filename, Options opts)
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
        else
            line = line.trimmed().simplified();

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
        else if (cmd == "usemtl")
            processUseMaterial(parts);
        else if (cmd == "mtllib")
            processMaterialLib(filename,parts);

    }

    if (mVertices.isEmpty())
        return false;

    if (opts.testFlag(VertexPoints)) {
        setupVertexDotMesh(model);
        return true;
    }

    if (opts.testFlag(NormalVectors)) {
        setupNormalMesh(model);
        return true;
    }

    if ((!mTextureName.isEmpty() || !mMaterialTextures.isEmpty()) && !mTextureCoords.isEmpty())
        setupTexturedMesh(model);
    else
        setupSimpleMesh(model);

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

    int v[4] = {-1,-1,-1,-1};
    int t[4] = {-1,-1,-1,-1};
    int n[4] = {-1,-1,-1,-1};

    for (int i=0; i<args.count(); ++i) {
        QStringList indexes = args.at(i).split("/");
        if (indexes.isEmpty())
            return;
        v[i] = indexes.at(0).toInt()-1;
        if (indexes.count() >= 2 && !indexes.at(1).isEmpty())
            t[i] = indexes.at(1).toInt()-1;
        if (indexes.count() >= 3 && !indexes.at(2).isEmpty())
            n[i] = indexes.at(2).toInt()-1;
    }

    auto addTriangle = [&](int a, int b, int c) {
        Triangle tri;
        tri.v[0] = v[a]; tri.v[1] = v[b]; tri.v[2] = v[c];
        tri.t[0] = t[a]; tri.t[1] = t[b]; tri.t[2] = t[c];
        tri.n[0] = n[a]; tri.n[1] = n[b]; tri.n[2] = n[c];
        tri.materialName = mCurrentMaterial;
        mTriangles << tri;

        mFaceVertexIndexes << tri.v[0] << tri.v[1] << tri.v[2];
        mFaceTextureIndexes << tri.t[0] << tri.t[1] << tri.t[2];
        mFaceNormalsIndexes << tri.n[0] << tri.n[1] << tri.n[2];
    };

    addTriangle(0,1,2);
    if (args.count() == 4)
        addTriangle(0,2,3);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dObjLoader::processTextureCoords(const QStringList &args)
{
    if (args.count() < 2)
        return;
    mTextureCoords << QPointF(args[0].toFloat(), args[1].toFloat());
}

//-------------------------------------------------------------------------------------------------
void Qtr3dObjLoader::processSmoothshading(const QStringList &args)
{
    Q_UNUSED(args);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dObjLoader::processUseMaterial(const QStringList &args)
{
    mCurrentMaterial = args.join(" ").trimmed();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dObjLoader::processMaterialLib(const QString &sourcefile, const QStringList &args)
{
    if (args.isEmpty())
        return;

    QString mathlibFile = addPath(sourcefile,args.join(" "));

    if (!QFileInfo::exists(mathlibFile))
        return;

    QFile f(mathlibFile);
    if (!f.open(QIODevice::ReadOnly))
        return;

    QTextStream in(&f);

    QString currentMaterial;
    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();
        int commentPos = line.indexOf('#');
        if (commentPos >= 0)
            line = line.mid(0, commentPos).trimmed().simplified();
        else
            line = line.simplified();

        if (line.isEmpty())
            continue;

        QStringList parts = line.split(" ");
        if (parts.isEmpty())
            continue;
        const QString cmd = parts.takeFirst();
        if (cmd == "newmtl")
            currentMaterial = parts.join(" ").trimmed();
        else if (cmd == "map_Kd")
            processMaterialTexture(mathlibFile, parts, currentMaterial);
    }
}

//-------------------------------------------------------------------------------------------------
void Qtr3dObjLoader::processMaterialTexture(const QString &matlibFilename, const QStringList &args, const QString &materialName)
{
    if (args.isEmpty())
        return;

    bool hasOptions = false;
    for (const QString &arg: args) {
        if (arg.startsWith("-")) {
            hasOptions = true;
            break;
        }
    }

    const QString textureFile = hasOptions ? args.last() : args.join(" ");
    QString textureName = addPath(matlibFilename, textureFile);
    if (!QFileInfo::exists(textureName))
        return;

    if (!materialName.isEmpty())
        mMaterialTextures[materialName] = textureName;
    if (mTextureName.isEmpty())
        mTextureName = textureName;
}

//-------------------------------------------------------------------------------------------------
static bool qtr3dValidIndex(int index, int count)
{
    return index >= 0 && index < count;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dObjLoader::setupTexturedMesh(Qtr3dModel &model)
{
    QMap<QString,QList<Triangle> > trianglesByTexture;
    QList<Triangle> untexturedTriangles;

    for (const auto &tri: mTriangles) {
        const QString textureName = mMaterialTextures.isEmpty() ? mTextureName : mMaterialTextures.value(tri.materialName);
        if (!textureName.isEmpty())
            trianglesByTexture[textureName] << tri;
        else
            untexturedTriangles << tri;
    }

    auto addTriangleVertices = [this](Qtr3dMesh *mesh, const Triangle &tri, bool textured) {
        for (int i=0; i<3; ++i) {
            if (!qtr3dValidIndex(tri.v[i], mVertices.count()))
                continue;
            const QVector3D pos = mVertices.at(tri.v[i]);
            const bool hasNormal = qtr3dValidIndex(tri.n[i], mNormals.count());
            const bool hasTexCoord = textured && qtr3dValidIndex(tri.t[i], mTextureCoords.count());

            if (hasTexCoord && hasNormal)
                mesh->addVertex(pos, mNormals.at(tri.n[i]), mTextureCoords.at(tri.t[i]).x(), mTextureCoords.at(tri.t[i]).y());
            else if (hasTexCoord)
                mesh->addVertex(pos, mTextureCoords.at(tri.t[i]).x(), mTextureCoords.at(tri.t[i]).y());
            else if (hasNormal)
                mesh->addVertex(pos, mNormals.at(tri.n[i]));
            else
                mesh->addVertex(pos);
        }
    };

    for (auto it = trianglesByTexture.constBegin(); it != trianglesByTexture.constEnd(); ++it) {
        QImage texture(it.key());
        if (texture.isNull()) {
            untexturedTriangles << it.value();
            continue;
        }

        Qtr3dMesh *mesh = model.context()->createMesh(false);
        mesh->setTexture(texture);
        mesh->startMesh(Qtr3d::Triangle);
        for (const auto &tri: it.value())
            addTriangleVertices(mesh, tri, true);
        mesh->endMesh();
        model.addMesh(mesh, true);
    }

    if (!untexturedTriangles.isEmpty()) {
        Qtr3dMesh *mesh = model.context()->createMesh(false);
        mesh->startMesh(Qtr3d::Triangle);
        mesh->setDefaultColor(model.material().ambient().mcolor);
        for (const auto &tri: untexturedTriangles)
            addTriangleVertices(mesh, tri, false);
        mesh->endMesh();
        model.addMesh(mesh, true);
    }
}

//-------------------------------------------------------------------------------------------------
void Qtr3dObjLoader::setupSimpleMesh(Qtr3dModel &model)
{
    Qtr3dMesh *mesh = model.context()->createMesh(false);
    mesh->startMesh(Qtr3d::Triangle);
    mesh->setDefaultColor(model.material().ambient().mcolor); // QColor("#293133"));
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
        mesh->addIndex(mFaceVertexIndexes.at(i),mFaceNormalsIndexes.at(i));

    mesh->endMesh();
    model.addMesh(mesh, true);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dObjLoader::setupVertexDotMesh(Qtr3dModel &model)
{
    Qtr3dMesh *mesh = model.context()->createMesh(false);
    mesh->setDefaultColor(Qt::white);
    mesh->startMesh(Qtr3d::Dot);

    bool colored = mVerticesColors.count() == mVertices.count();

    // Vertices
    for (int i=0; i<mVertices.count(); i++) {
        if (colored)
            mesh->addVertex(mVertices.at(i),mVerticesColors.at(i));
        else
            mesh->addVertex(mVertices.at(i));
    }

    mesh->endMesh();
    model.addMesh(mesh);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dObjLoader::setupNormalMesh(Qtr3dModel &model)
{
    if (mNormals.isEmpty())
        return;

    Qtr3dMesh *mesh = model.context()->createMesh(false);
    mesh->setDefaultColor(Qt::red);
    mesh->startMesh(Qtr3d::Line);

    for (int i=0; i<mFaceVertexIndexes.count(); i++) {
        QVector3D v = mVertices.at(mFaceVertexIndexes.at(i));
        QVector3D n = mNormals.at(mFaceNormalsIndexes.at(i));


        mesh->addVertex(v);
        mesh->addVertex(v + (50*n));

    }


    mesh->endMesh();
    model.addMesh(mesh);
}

