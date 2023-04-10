
#include <QJsonDocument>
#include <QVariant>
#include <QQuaternion>
#include "qtr3dbinreader.h"
#include "qtr3dglbloader.h"
#include "../qtr3dmesh.h"
#include "../qtr3dtexturefactory.h"

//-------------------------------------------------------------------------------------------------
bool Qtr3dGlbLoader::supportsFile(const QString &filename)
{
    return filename.toLower().endsWith(".glb");
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dGlbLoader::loadFile(Qtr3dModel &model, const QString &filename)
{
    Qtr3dGlbLoader loader;
    return loader.loadModel(model,filename);
}

//-------------------------------------------------------------------------------------------------
Qtr3dGlbLoader::Qtr3dGlbLoader()
{
}
Qtr3dGlbLoader::~Qtr3dGlbLoader() = default;

//-------------------------------------------------------------------------------------------------
bool Qtr3dGlbLoader::loadModel(Qtr3dModel &model, const QString &filename)
{
    QByteArray header = fileHeader(filename,100);
    if (header.isEmpty())
        return returnError("Empty file");

    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly))
        return returnError("Can't open file");

    Qtr3dBinReader reader(f.readAll());

    // https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#glb-file-format-specification
    quint32 magic   = reader.readUint32();
    quint32 version = reader.readUint32();
    quint32 length  = reader.readUint32();
    Q_UNUSED(length);

    if (magic !=  0x46546C67) // yea... that's why it's called "magic"
        return returnError("Invalid GLB Magic number");

    mModel   = &model;

    if (version == 2)
        return loadGlbv2(reader);

    return returnError("Unknown GLB Version number (only V2 is supported)");
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dGlbLoader::loadGlbv2(Qtr3dBinReader &binReader)
{
    while (!binReader.atEnd()) {
        if (!parseJunkv2(binReader))
            break;
    }

    // splitAccessors(mJsonStruct["accessors"].toList());
    /*
      1) load scenes
      2) load nodes
      3) load meshes
    */

    int sceneIndex = mJsonStruct.value("scene",0).toInt();
    QVariantList scenes = mJsonStruct["scenes"].toList();
    if (sceneIndex >= scenes.count())
        return returnError("Invalid Scene");

    QVariantList sceneNodes = scenes[sceneIndex].toMap()["nodes"].toList();
    QVariantList allNodes   = mJsonStruct["nodes"].toList();
    QVariantList allMeshes  = mJsonStruct["meshes"].toList();

    for (int m=0; m<allMeshes.count(); m++)
        createMesh(allMeshes[m].toMap(), m);

    for (auto const &node: sceneNodes) {
        int nodeIndex = node.toInt();
        if (nodeIndex < 0 || nodeIndex >= allNodes.count())
            return returnError("Invalid Scene");
        createNode(allNodes[nodeIndex].toMap());
    }

    return true;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dGlbLoader::parseJunkv2(Qtr3dBinReader &binReader)
{
    quint32    chunkLength = binReader.readUint32();
    quint32    chunkType   = binReader.readUint32();
    QByteArray chunkData   = binReader.readBlock(chunkLength);

    // https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#table-chunktypes
    switch(chunkType) {
    case 0x4E4F534A: parseJsonJunk(chunkData);   break;
    case 0x004E4942: parseBinaryJunk(chunkData); break;
    default: break;
    }
    return !binReader.error();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dGlbLoader::parseJsonJunk(const QByteArray &data)
{
    mJsonStruct = QJsonDocument::fromJson(data).toVariant().toMap();

    //QFile f("/tmp/test.json");
    //if (f.open(QIODevice::WriteOnly))
    //        f.write(QJsonDocument::fromVariant(mJsonStruct).toJson(QJsonDocument::Indented));
}

//-------------------------------------------------------------------------------------------------
void Qtr3dGlbLoader::parseBinaryJunk(const QByteArray &data)
{
    mAccessors << data;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dGlbLoader::createNode(const QVariantMap &nodeInfo, Qtr3dModel::Node *parent)
{
    // Optional... node could contain ONLY Children..
    int meshIndex = nodeInfo.value("mesh",-1).toInt();
    QVariantList meshInfos = mJsonStruct["meshes"].toList();

    QVariantList translation = nodeInfo["translation"].toList();
    QVariantList rotation    = nodeInfo["rotation"].toList();
    QVariantList scale       = nodeInfo["scale"].toList();

    QMatrix4x4 m(1.0, 0.0, 0.0, 0.0,
                 0.0, 1.0, 0.0, 0.0,
                 0.0, 0.0, 1.0, 0.0,
                 0.0, 0.0, 0.0, 1.0);

    if (translation.count() == 3) // x, y, z
       m.translate(translation[0].toFloat(),
                   translation[1].toFloat(),
                   translation[2].toFloat());

    if (rotation.count() == 4) {   // x, y, z, w
        QVector3D v(rotation[0].toFloat(), rotation[1].toFloat(), rotation[2].toFloat());
        QQuaternion q(rotation[3].toFloat(),v); // v.normalized());
        m.rotate(q);
    }
    if (scale.count() == 3) // x, y, z
        m.scale(scale[0].toFloat(), scale[1].toFloat(), scale[2].toFloat());

    /*
       In JSON, the Meshes are defined as "Meshes contains multiple Primitives"
       In Qtr3d: all "Primitives" are "Meshes"
       .... so...
       ..... here we have to revere translate the Json-Mesh-Index to the Primitive-Index (aka Mesh-Index)
       in Qtr3dModel..
    */
    Qtr3dMeshes nodeMeshes;
    if (meshIndex >= 0) {
        QList<int> qtr3dMeshIndexes = mJsonMeshes2qtrmeshes[meshIndex];
        for (auto qtrMeshIndex : qtr3dMeshIndexes)
            nodeMeshes << mModel->meshes()[qtrMeshIndex];
    }
    Qtr3dModel::Node *node = mModel->createNode(nodeMeshes,m, parent);

    QVariantList childs       = nodeInfo["children"].toList();
    QVariantList allNodes     = mJsonStruct["nodes"].toList();
    for (auto const &nodeInfo: childs) {
        int nodeIndex = nodeInfo.toInt();
        if (nodeIndex < 0 || nodeIndex >= allNodes.count())
            return;
        createNode(allNodes[nodeIndex].toMap(), node);
    }
}


//-------------------------------------------------------------------------------------------------
void Qtr3dGlbLoader::createMesh(const QVariantMap &meshInfo, int meshIndex)
{
    QVariantList primitiveInfos = meshInfo["primitives"].toList();

    for (int i=0; i<primitiveInfos.count(); i++) {
        int indicesAcsessorIndex  = primitiveInfos[i].toMap().value("indices",-1).toInt();
        int materialIndex         = primitiveInfos[i].toMap().value("material",-1).toInt();
        int textCoordIndex = -1;
        QImage texture = loadTexture(materialIndex,textCoordIndex);
        if (!texture.isNull() && textCoordIndex < 0) // Default TextureCoordIndex to 0 (Reference "duck.glb")
            textCoordIndex = 0;

        int positionAccessorIndex = primitiveInfos[i].toMap()["attributes"].toMap().value("POSITION",-1).toInt();
        int normalAccessorIndex   = primitiveInfos[i].toMap()["attributes"].toMap().value("NORMAL",-1).toInt();
        int textcoordAccessorIndex= primitiveInfos[i].toMap()["attributes"].toMap().value(QString("TEXCOORD_%1").arg(textCoordIndex),-1).toInt();

        Q_ASSERT(indicesAcsessorIndex >= 0);
        Q_ASSERT(normalAccessorIndex >= 0);

        Qtr3dGeometry *buffer = nullptr;
        if (textcoordAccessorIndex >= 0) {
            buffer = loadTexturedMesh(accessorInfo(positionAccessorIndex),
                                      accessorInfo(indicesAcsessorIndex),
                                      accessorInfo(normalAccessorIndex),
                                      accessorInfo(textcoordAccessorIndex),
                                      texture);
        } else {
            buffer = loadColoredMesh(accessorInfo(positionAccessorIndex),
                                     accessorInfo(indicesAcsessorIndex),
                                     accessorInfo(normalAccessorIndex));
        }
        if (buffer) {
            mJsonMeshes2qtrmeshes[meshIndex] << mModel->meshes().count()-1;
        } else {
            qWarning() << "Qtr3dGlbLoader::createMesh: Invalid Mesh" << meshInfo << primitiveInfos;
        }
    }
}

//-------------------------------------------------------------------------------------------------
QByteArray Qtr3dGlbLoader::bufferView(int index) const
{
    QVariantList bufferInfos = mJsonStruct["bufferViews"].toList();
    if (index < 0 || index >= bufferInfos.count())
        return QByteArray();

    int bufferIndex= bufferInfos[index].toMap()["buffer"].toInt();
    int byteLength = bufferInfos[index].toMap()["byteLength"].toInt();
    int byteOffset = bufferInfos[index].toMap()["byteOffset"].toInt();

    if (byteLength <= 0)
        return QByteArray();

    if (bufferIndex < 0 || bufferIndex >= mAccessors.count())
        return QByteArray();

    if ((byteOffset + byteLength) > mAccessors[bufferIndex].length())
        return QByteArray();

    return mAccessors[bufferIndex].mid(byteOffset,byteLength);
}

//-------------------------------------------------------------------------------------------------
QVariantMap Qtr3dGlbLoader::accessorInfo(int index) const
{
    QVariantList accessors = mJsonStruct["accessors"].toList();
    if (index < 0 || index >= accessors.count())
        return QVariantMap();
    return accessors[index].toMap();
}

//-------------------------------------------------------------------------------------------------
Qtr3dMesh *Qtr3dGlbLoader::loadTexturedMesh(const QVariantMap &positionInfo, const QVariantMap &faceInfo, const QVariantMap &normalInfo, const QVariantMap &textCoordInfo, const QImage &texture)
{
    if (positionInfo.isEmpty() || faceInfo.isEmpty())
        return nullptr;

    QList<QVector3D> points = loadVectors(
                positionInfo["componentType"].toInt(),
            positionInfo["byteOffset"].toInt(),
            positionInfo["count"].toInt(),
            bufferView(positionInfo["bufferView"].toInt())
            );

    QList<int> faceIndexes = loadFaceIndexes(
                faceInfo["componentType"].toInt(),
            faceInfo["byteOffset"].toInt(),
            faceInfo["count"].toInt(),
            bufferView(faceInfo["bufferView"].toInt())
            );

    QList<QVector3D> normVectors = loadVectors(
                normalInfo["componentType"].toInt(),
            normalInfo["byteOffset"].toInt(),
            normalInfo["count"].toInt(),
            bufferView(normalInfo["bufferView"].toInt())
            );

    QList<QPointF> textureCoords = loadTextureCoords(
                textCoordInfo["componentType"].toInt(),
            textCoordInfo["byteOffset"].toInt(),
            textCoordInfo["count"].toInt(),
            bufferView(textCoordInfo["bufferView"].toInt())
            );

    auto *mesh = mModel->context()->createMesh(false);
    mesh->setTexture(texture.mirrored());
    mesh->startMesh(Qtr3d::Triangle);
    for (int vi=0; vi < points.count(); vi++)
        mesh->addVertex(points[vi],normVectors[vi], textureCoords[vi].y(),textureCoords[vi].x());
    for (auto i: faceIndexes)
        mesh->addIndex(i);

    mesh->endMesh();
    mModel->addMesh(mesh);

    return mesh;
}

//-------------------------------------------------------------------------------------------------
Qtr3dMesh *Qtr3dGlbLoader::loadColoredMesh(const QVariantMap &positionInfo, const QVariantMap &faceInfo, const QVariantMap &normalInfo)
{
    if (positionInfo.isEmpty() || faceInfo.isEmpty())
        return nullptr;

    QList<QVector3D> points = loadVectors(
                positionInfo["componentType"].toInt(),
            positionInfo["byteOffset"].toInt(),
            positionInfo["count"].toInt(),
            bufferView(positionInfo["bufferView"].toInt())
            );

    QList<int> faceIndexes = loadFaceIndexes(
                faceInfo["componentType"].toInt(),
            faceInfo["byteOffset"].toInt(),
            faceInfo["count"].toInt(),
            bufferView(faceInfo["bufferView"].toInt())
            );

    QList<QVector3D> normVectors = loadVectors(
                normalInfo["componentType"].toInt(),
            normalInfo["byteOffset"].toInt(),
            normalInfo["count"].toInt(),
            bufferView(normalInfo["bufferView"].toInt())
            );


    auto *mesh = mModel->context()->createMesh(false);
    mesh->startMesh(Qtr3d::Triangle);
    mesh->setDefaultColor(Qt::white);
    for (int vi=0; vi < points.count(); vi++)
        mesh->addVertex(points[vi],normVectors[vi]);
    for (auto i: faceIndexes)
        mesh->addIndex(i);

    mesh->endMesh();
    mModel->addMesh(mesh);
    return mesh;
}


//-------------------------------------------------------------------------------------------------
QList<QVector3D> Qtr3dGlbLoader::loadVectors(int componentType, int offset, int count, const QByteArray &buffer) const
{
    if (componentType != GL_FLOAT)
        return QList<QVector3D>();

    QList<QVector3D> ret;
    Qtr3dBinReader reader(buffer);
    reader.skip(offset);

    while(count) {
        float x = reader.readFloat();
        float y = reader.readFloat();
        float z = reader.readFloat();
        ret << QVector3D(x,y,z);
        count--;
    }

    Q_ASSERT(reader.parsedBytes() <= buffer.length());
    return ret;
}

//-------------------------------------------------------------------------------------------------
QList<int> Qtr3dGlbLoader::loadFaceIndexes(int componentType, int offset, int count, const QByteArray &buffer) const
{
    if (componentType != GL_UNSIGNED_SHORT)
        return QList<int>();

    QList<int> ret;
    Qtr3dBinReader reader(buffer);
    reader.skip(offset);

    while(count) {
        ret << reader.readUint16();
        count--;
    }

    Q_ASSERT(reader.parsedBytes() <= buffer.length());
    return ret;
}

//-------------------------------------------------------------------------------------------------
QList<QPointF> Qtr3dGlbLoader::loadTextureCoords(int componentType, int offset, int count, const QByteArray &buffer) const
{
    if (componentType != GL_FLOAT)
        return QList<QPointF>();

    QList<QPointF> ret;
    Qtr3dBinReader reader(buffer);

    reader.skip(offset);

    while(count) {
        ret << QPointF(reader.readFloat(), reader.readFloat());
        count--;
    }

    Q_ASSERT(reader.parsedBytes() <= buffer.length());
    return ret;
}

//-------------------------------------------------------------------------------------------------
QImage Qtr3dGlbLoader::loadTexture(int materialIndex, int &texCoord) const
{
    QImage ret;
    if (materialIndex < 0)
        return ret;

    texCoord = -1;

    QVariantMap material = mJsonStruct["materials"].toList()[materialIndex].toMap();
    QVariantMap metallic = material["pbrMetallicRoughness"].toMap();

    if (metallic.contains("baseColorTexture")) {
        int textureIndex = metallic["baseColorTexture"].toMap().value("index",-1).toInt();

        if (textureIndex < 0)
            return ret;

        texCoord         = metallic["baseColorTexture"].toMap().value("texCoord",-1).toInt();
        int imageIndex   = mJsonStruct["textures"].toList()[textureIndex].toMap()["source"].toInt();
        int bufferIndex  = mJsonStruct["images"].toList()[imageIndex].toMap()["bufferView"].toInt();
        ret = QImage::fromData(bufferView(bufferIndex));
    }

    return ret;
}




