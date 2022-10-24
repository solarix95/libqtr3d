
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
bool Qtr3d3dsLoader::loadFile(Qtr3dVertexMesh &mesh, const QString &filename)
{
    Qtr3d3dsLoader loader;
    return loader.loadMesh(mesh,filename);
}

//-------------------------------------------------------------------------------------------------}
Qtr3d3dsLoader::Qtr3d3dsLoader()
{
}

//-------------------------------------------------------------------------------------------------}
Qtr3d3dsLoader::~Qtr3d3dsLoader() = default;

//-------------------------------------------------------------------------------------------------}
bool Qtr3d3dsLoader::loadMesh(Qtr3dVertexMesh &mesh, const QString &filename)
{
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly))
        return false;

    Qtr3dBinReader reader(f.readAll());

    QList<quint16> junkContext;
    junkContext << reader.readUint16();

    if (junkContext.first() == 0x4D4D)
        processMainJunk(reader);

    return true;
}

//-------------------------------------------------------------------------------------------------}
QList<quint16> Qtr3d3dsLoader::processJunk(Qtr3dBinReader &reader, const QList<quint16> &contexts)
{
    qint32 junkSize = reader.readInt32();

}

//-------------------------------------------------------------------------------------------------}
void Qtr3d3dsLoader::processMainJunk(Qtr3dBinReader &reader)
{
    /* qint32 junkSize = */ reader.readInt32();
    if (!toJunk(0x3D3D, reader))
        return;
    processEditorJunk(reader);
}

//-------------------------------------------------------------------------------------------------}
void Qtr3d3dsLoader::processEditorJunk(Qtr3dBinReader &reader)
{
    /* qint32 junkSize = */ reader.readInt32();
    while (true) {
        if (!toJunk(0x4000, reader))
            return;
        processObjectJunk(reader);
    }
}

//-------------------------------------------------------------------------------------------------}
void Qtr3d3dsLoader::processObjectJunk(Qtr3dBinReader &reader)
{
    // http://paulbourke.net/dataformats/3ds/
    /* qint32 junkSize = */ reader.readInt32();
    /* QString header  = */ reader.readAsciiZ();
    if (!toJunk(0x4100, reader))
        return;
    processMeshJunk(reader);
}

//-------------------------------------------------------------------------------------------------}
void Qtr3d3dsLoader::processMeshJunk(Qtr3dBinReader &reader)
{
    /* qint32 junkSize = */ reader.readInt32();
    if (!toJunk(0x4110, reader))
        return;
    processVerticesJunk(reader);
}

//-------------------------------------------------------------------------------------------------}
void Qtr3d3dsLoader::processVerticesJunk(Qtr3dBinReader &reader)
{
    /* qint32 junkSize = */ reader.readInt32();
    quint16 numVertices = reader.readUint16();
    while (numVertices > 0) {
        qDebug() << reader.readFloat() << reader.readFloat() << reader.readFloat();
        numVertices--;
    }
}

//-------------------------------------------------------------------------------------------------}
bool Qtr3d3dsLoader::toJunk(quint16 junkIdent, Qtr3dBinReader &reader)
{
    quint16 nextJunk;
    qint32  nextSize;
    while ((nextJunk = reader.readUint16()) != junkIdent && !reader.error()) {
        std::cout << "Skipping: " << std::hex << nextJunk;
        reader.skip((nextSize = (reader.readInt32()-6)));
        std::cout << " of size: " << std::dec << nextSize << std::endl;
    }
    if (!reader.error())
        std::cout << "Entering: " << std::hex << junkIdent << std::endl;
    return !reader.error();
}



