
#include <iostream>
#include <iomanip>
#include <QFile>
#include <libqtr3d/loader/qtr3dbinreader.h>
#include "qtr3d3dsloader.h"

//-------------------------------------------------------------------------------------------------}
bool Qtr3d3dsLoader::supportsFile(const QString &filename)
{
    return filename.toLower().endsWith(".3ds");
}

//-------------------------------------------------------------------------------------------------}
bool Qtr3d3dsLoader::loadFile(Qtr3dModel &model, const QString &filename, Qtr3dGeometryBufferFactory &factory)
{
    Qtr3d3dsLoader loader;
    return loader.loadModel(model,filename, factory);
}

//-------------------------------------------------------------------------------------------------}
Qtr3d3dsLoader::Qtr3d3dsLoader()
 : mMesh(nullptr)
{
}

//-------------------------------------------------------------------------------------------------}
Qtr3d3dsLoader::~Qtr3d3dsLoader() = default;

//-------------------------------------------------------------------------------------------------}
bool Qtr3d3dsLoader::loadModel(Qtr3dModel &model, const QString &filename, Qtr3dGeometryBufferFactory &factory)
{
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly))
        return false;

    mMesh = factory.createVertexMesh();

    mMesh->startMesh(Qtr3dVertexMesh::Triangle);

    Qtr3dBinReader reader(f.readAll());

    mParserHooks["4D4D"] = [&](const QString &url, int junkSize){
        processNextJunks(reader,url,junkSize);
    };
    mParserHooks["4D4D-3D3D"] = [&](const QString &url, int junkSize){
        processNextJunks(reader,url,junkSize);
    };
    mParserHooks["4D4D-3D3D-AFFF"] = [&](const QString &url, int junkSize){
        processNextJunks(reader,url,junkSize);
    };
    mParserHooks["4D4D-3D3D-AFFF-A000"] = [&](const QString &url, int junkSize){
        processMaterialNameJunk(reader);
    };
    mParserHooks["4D4D-3D3D-4000"] = [&](const QString &url, int junkSize){
        processObjectJunk(reader,url,junkSize);
    };
    mParserHooks["4D4D-3D3D-4000-4100"] = [&](const QString &url, int junkSize){
       processNextJunks(reader,url,junkSize);
    };
    mParserHooks["4D4D-3D3D-4000-4100-4110"] = [&](const QString &, int){               // TRI_VERTEXL
        processVerticesJunk(reader);
    };
    mParserHooks["4D4D-3D3D-4000-4100-4120"] = [&](const QString &url, int junkSize){   // TRI_FACEL1
        processFacesJunk(reader, url, junkSize);
    };
    mParserHooks["4D4D-3D3D-4000-4100-4120-4130"] = [&](const QString &, int){          // TRI_MATERIAL
        processMaterialJunk(reader);
    };
    mParserHooks["4D4D-3D3D-4000-4100-4140"] = [&](const QString &, int){               // TRI_MAPPINGCOORS
        processMappingCoordJunk(reader);
    };

    processNextJunk(reader,"");
    mMesh->endMesh();

    model.addGeometry(mMesh);
    return true;
}

//-------------------------------------------------------------------------------------------------}
qint32 Qtr3d3dsLoader::processNextJunk(Qtr3dBinReader &reader, const QString &parentURL)
{
    qint32  enterPos   = reader.parsedBytes();
    quint16 nextJunk   = reader.readUint16();
    qint32  junkSize   = reader.readInt32();
    QString junkUrl    = QString("%1%2%3")
                            .arg(parentURL)
                            .arg(parentURL.isEmpty() ? "":"-")
                            .arg(nextJunk,4,16,QChar('0')).toUpper();

    if (mParserHooks.contains(junkUrl))
        mParserHooks[junkUrl](junkUrl,junkSize-6);

    reader.skip(junkSize - (reader.parsedBytes() - enterPos));
    return junkSize;
}

//-------------------------------------------------------------------------------------------------}
void Qtr3d3dsLoader::processNextJunks(Qtr3dBinReader &reader, const QString &parentURL, int junkSizes)
{
    while (junkSizes > 0) {
        junkSizes -= processNextJunk(reader,parentURL);
    }
    Q_ASSERT(junkSizes == 0);
}

//-------------------------------------------------------------------------------------------------}
void Qtr3d3dsLoader::processObjectJunk(Qtr3dBinReader &reader, const QString &parentURL, int junkSizes)
{
    // http://paulbourke.net/dataformats/3ds/
    qint32  enterPos   = reader.parsedBytes();
    QString header     = reader.readAsciiZ();

    mObjectVertices.clear();
    mObjectFaces.clear();
    mTextureMappings.clear();
    mTextureFaces.clear();
    mMaterialNames.clear();

    processNextJunks(reader,parentURL,junkSizes-(reader.parsedBytes()-enterPos));

    setupMesh();
}

//-------------------------------------------------------------------------------------------------}
void Qtr3d3dsLoader::processMaterialNameJunk(Qtr3dBinReader &reader)
{
    QString matName     = reader.readAsciiZ();
    mMaterialNames << matName;
}

//-------------------------------------------------------------------------------------------------}
void Qtr3d3dsLoader::processVerticesJunk(Qtr3dBinReader &reader)
{
    quint16 numVertices = reader.readUint16();
    while (numVertices > 0) {
        mObjectVertices << QVector3D(reader.readFloat(),reader.readFloat(),reader.readFloat());
        numVertices--;
    }
}

//-------------------------------------------------------------------------------------------------}
void Qtr3d3dsLoader::processFacesJunk(Qtr3dBinReader &reader, const QString &parentURL, int junkSizes)
{
    qint32  enterPos   = reader.parsedBytes();
    quint16 numFaces   = reader.readUint16();
    while (numFaces > 0) {
        QList<int> face;
        face << reader.readUint16(); // A
        face << reader.readUint16(); // B
        face << reader.readUint16(); // C
        mObjectFaces << face;
        reader.readUint16(); // skip Face Info
        numFaces--;
    }

    int subJunksSize = junkSizes - (reader.parsedBytes() - enterPos);
    processNextJunks(reader,parentURL,subJunksSize);
}

//-------------------------------------------------------------------------------------------------}
void Qtr3d3dsLoader::processMaterialJunk(Qtr3dBinReader &reader)
{
    QString materialName = reader.readAsciiZ();
    quint16  numFaces    = reader.readUint16();

    qDebug() << "HANDLE MATERIAL" << materialName;
    while (numFaces > 0) {
        mTextureFaces << reader.readUint16();
        numFaces--;
    }
}

//-------------------------------------------------------------------------------------------------}
void Qtr3d3dsLoader::processMappingCoordJunk(Qtr3dBinReader &reader)
{
    quint16 numVertices = reader.readUint16();
    while (numVertices > 0) {
        float  uvalue      = reader.readFloat();
        float  vvalue      = reader.readFloat();
        mTextureMappings << QPointF(uvalue,vvalue);
        numVertices--;
    }
}

//-------------------------------------------------------------------------------------------------}
void Qtr3d3dsLoader::setupMesh()
{
    Q_ASSERT(mMesh);

    for (auto face : mObjectFaces) {
        QVector3D normal = QVector3D::crossProduct(mObjectVertices[face[1]]-mObjectVertices[face[0]],mObjectVertices[face[2]]-mObjectVertices[face[1]]).normalized();
        mMesh->addVertex(mObjectVertices[face[0]],normal);
        mMesh->addVertex(mObjectVertices[face[1]],normal);
        mMesh->addVertex(mObjectVertices[face[2]],normal);

        // qDebug() << mObjectVertices[face[0]] << mObjectVertices[face[1]] << mObjectVertices[face[2]];
    }
}





