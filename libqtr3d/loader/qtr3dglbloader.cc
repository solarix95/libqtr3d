
#include <QJsonDocument>
#include <QVariant>
#include "qtr3dbinreader.h"
#include "qtr3dglbloader.h"
#include "../qtr3dvertexmesh.h"
#include "../qtr3dtexturedmesh.h"
#include "../qtr3dtexturefactory.h"

//-------------------------------------------------------------------------------------------------
bool Qtr3dGlbLoader::supportsFile(const QString &filename)
{
    return filename.toLower().endsWith(".glb");
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dGlbLoader::loadFile(Qtr3dModel &model, const QString &filename, Qtr3dGeometryBufferFactory &factory)
{
    Qtr3dGlbLoader loader;
    return loader.loadModel(model,filename, factory);
}

//-------------------------------------------------------------------------------------------------
Qtr3dGlbLoader::Qtr3dGlbLoader()
{
}
Qtr3dGlbLoader::~Qtr3dGlbLoader() = default;

//-------------------------------------------------------------------------------------------------
bool Qtr3dGlbLoader::loadModel(Qtr3dModel &model, const QString &filename, Qtr3dGeometryBufferFactory &factory)
{
    QByteArray header = fileHeader(filename,100);
    if (header.isEmpty())
        return false;

    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly))
        return false;

    Qtr3dBinReader reader(f.readAll());

    // https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#glb-file-format-specification
    quint32 magic   = reader.readUint32();
    quint32 version = reader.readUint32();
    quint32 length  = reader.readUint32();

    if (magic !=  0x46546C67) { // yea... that's why it's called "magic"
        return false;
    }

    mModel   = &model;
    mFactory = &factory;

    if (version == 2)
        return loadGlbv2(reader);

    return false;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dGlbLoader::loadGlbv2(Qtr3dBinReader &binReader)
{
    while (!binReader.atEnd()) {
        if (!parseJunkv2(binReader))
            break;
    }

    // splitAccessors(mJsonStruct["accessors"].toList());
    QVariantList meshInfos = mJsonStruct["meshes"].toList();
    if (meshInfos.count() == 0)
        return false;

    for (int i=0; i<meshInfos.count(); i++) {
        createMesh(meshInfos[i].toMap());
    }

    return false;
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
}

//-------------------------------------------------------------------------------------------------
void Qtr3dGlbLoader::parseBinaryJunk(const QByteArray &data)
{
    mAccessors << data;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dGlbLoader::createMesh(const QVariantMap &meshInfo)
{
    QVariantList primitiveInfos = meshInfo["primitives"].toList();
    for (int i=0; i<primitiveInfos.count(); i++) {
        int indicesAcsessorIndex  = primitiveInfos[i].toMap()["indices"].toInt();
        int materialIndex         = primitiveInfos[i].toMap()["material"].toInt();
        int textCoordIndex = -1;
        QImage texture = loadTexture(materialIndex,textCoordIndex);

        int positionAccessorIndex = primitiveInfos[i].toMap()["attributes"].toMap()["POSITION"].toInt();
        int normalAccessorIndex   = primitiveInfos[i].toMap()["attributes"].toMap()["NORMAL"].toInt();
        int textcoordAccessorIndex= primitiveInfos[i].toMap()["attributes"].toMap()[QString("TEXCOORD_%1").arg(textCoordIndex)].toInt();
        loadMesh(accessorInfo(positionAccessorIndex),
                 accessorInfo(indicesAcsessorIndex),
                 accessorInfo(textcoordAccessorIndex),
                 texture);
        qDebug() << positionAccessorIndex << indicesAcsessorIndex << normalAccessorIndex << textcoordAccessorIndex;
    }
}

//-------------------------------------------------------------------------------------------------
void Qtr3dGlbLoader::splitAccessors(const QVariantList &bufferInfos)
{
    for (int i=0; i<bufferInfos.count(); i++) {
        takeNextAccessor(bufferInfos[i].toMap());
    }
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dGlbLoader::takeNextAccessor(const QVariantMap &bufferInfo)
{
    QString structType     = bufferInfo["type"].toString().toLower();
    int     count          = bufferInfo["count"].toInt();
    int     componentType  = bufferInfo["componentType"].toInt();
    int     componentSize  = 0;
    int     componentCount = 0;

    switch(componentType) {
    case GL_FLOAT          : componentSize = sizeof(qreal);    break;
    case GL_DOUBLE         : componentSize = sizeof(double);   break;
    case GL_UNSIGNED_INT   : componentSize = sizeof(quint32);  break;
    case GL_INT            : componentSize = sizeof(qint32);   break;
    case GL_UNSIGNED_SHORT : componentSize = sizeof(quint16);  break;
    case GL_SHORT          : componentSize = sizeof(qint16);   break;
    case GL_UNSIGNED_BYTE  : componentSize = sizeof(quint8);  break;
    case GL_BYTE           : componentSize = sizeof(qint8);   break;
    default: componentSize = 0;
    }


    if (structType == "vec3")
        componentCount = 3;
    else if (structType == "vec2")
        componentCount = 2;
    else if (structType == "scalar")
        componentCount = 1;

    int AccessorSize = componentCount * componentSize * count;
    qDebug() << "TAKING" << AccessorSize << mAccessors.count();
    // mAccessors.remove(0,AccessorSize);
    return true;
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
void Qtr3dGlbLoader::loadMesh(const QVariantMap &positionInfo, const QVariantMap &faceInfo, const QVariantMap &textCoordInfo, const QImage &texture)
{
    if (positionInfo.isEmpty() || faceInfo.isEmpty())
        return;


    QList<QVector3D> points = loadVectors(
                positionInfo["componentType"].toInt(),
            positionInfo["count"].toInt(),
            bufferView(positionInfo["bufferView"].toInt())
            );

    QList<int> faceIndexes = loadFaceIndexes(
                faceInfo["componentType"].toInt(),
            faceInfo["count"].toInt(),
            bufferView(faceInfo["bufferView"].toInt())
            );

    QList<QPointF> textureCoords = loadTextureCoords(
                textCoordInfo["componentType"].toInt(),
                textCoordInfo["count"].toInt(),
                bufferView(textCoordInfo["bufferView"].toInt())
            );


     mModel->texturesFactory()->texture(texture,"test");

     auto *mesh = mFactory->createTexturedMesh();
     mesh->startMesh("test");
     for (int vi=0; vi < points.count(); vi++)
         mesh->addVertex(points[vi],textureCoords[vi].y(),textureCoords[vi].x());
     for (auto i: faceIndexes)
         mesh->addIndex(i);

     mesh->endMesh();
     mModel->addGeometry(mesh);


     /*
    auto *mesh = mFactory->createVertexMesh();
    mesh->setDefaultColor(Qt::white);
    mesh->startMesh(Qtr3dGeometryBuffer::Triangle);

    for (int vi=0; vi < points.count(); vi++) {
        QColor c = QColor::fromRgb(texture.pixel(texture.width() * textureCoords[vi].y(), texture.height() * textureCoords[vi].x()));
        mesh->addVertex(points[vi],c);
    }

   // for (auto v: points)
   //     mesh->addVertex(v,QColor(127 + (qrand()%127), 127 + (qrand()%127),127 + (qrand()%127)));
    for (auto i: faceIndexes)
        mesh->addIndex(i);
    mesh->endMesh();
    mModel->addGeometry(mesh);
    */
}



//-------------------------------------------------------------------------------------------------
QList<QVector3D> Qtr3dGlbLoader::loadVectors(int componentType, int count, const QByteArray &buffer) const
{
    if (componentType != GL_FLOAT)
        return QList<QVector3D>();

    QList<QVector3D> ret;
    Qtr3dBinReader reader(buffer);

    while(count) {
        ret << QVector3D(reader.readFloat(), reader.readFloat(), reader.readFloat());
        count--;
    }

    Q_ASSERT(reader.parsedBytes() == buffer.length());
    return ret;
}

//-------------------------------------------------------------------------------------------------
QList<int> Qtr3dGlbLoader::loadFaceIndexes(int componentType, int count, const QByteArray &buffer) const
{
    if (componentType != GL_UNSIGNED_SHORT)
        return QList<int>();

    QList<int> ret;
    Qtr3dBinReader reader(buffer);

    while(count) {
        ret << reader.readUint16();
        count--;
    }

    Q_ASSERT(reader.parsedBytes() == buffer.length());
    return ret;
}

//-------------------------------------------------------------------------------------------------
QList<QPointF> Qtr3dGlbLoader::loadTextureCoords(int componentType, int count, const QByteArray &buffer) const
{
    if (componentType != GL_FLOAT)
        return QList<QPointF>();

    QList<QPointF> ret;
    Qtr3dBinReader reader(buffer);

    while(count) {
        ret << QPointF(reader.readFloat(), reader.readFloat());
        count--;
    }

    Q_ASSERT(reader.parsedBytes() == buffer.length());
    return ret;
}

//-------------------------------------------------------------------------------------------------
QImage Qtr3dGlbLoader::loadTexture(int materialIndex, int &texCoord) const
{
    QImage ret;
    texCoord = -1;

    QVariantMap material = mJsonStruct["materials"].toList()[materialIndex].toMap();
    QVariantMap metallic = material["pbrMetallicRoughness"].toMap();

    if (metallic.contains("baseColorTexture")) {
        int textureIndex = metallic["baseColorTexture"].toMap()["index"].toInt();
        texCoord         = metallic["baseColorTexture"].toMap()["texCoord"].toInt();
        int imageIndex   = mJsonStruct["textures"].toList()[textureIndex].toMap()["source"].toInt();
        int bufferIndex  = mJsonStruct["images"].toList()[imageIndex].toMap()["bufferView"].toInt();
        ret = QImage::fromData(bufferView(bufferIndex));
    }

    return ret;
}




