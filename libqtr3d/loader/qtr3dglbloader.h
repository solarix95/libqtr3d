#ifndef QTR3DGLBLOADER_H
#define QTR3DGLBLOADER_H

#include <QString>
#include <QFile>
#include <QStringList>
#include <QList>
#include <QByteArray>
#include <QMap>
#include <QVariantMap>
#include <QVariantList>
#include "qtr3dmodelloader.h"

class Qtr3dBinReader;

class Qtr3dGlbLoader : public Qtr3dModelLoader
{
public:
    static bool supportsFile(const QString &filename);
    static bool loadFile(Qtr3dModel &model, const QString &filename);

    Qtr3dGlbLoader();
    virtual ~Qtr3dGlbLoader();

    virtual bool loadModel(Qtr3dModel &model, const QString &filename);

protected:

private:
    bool loadGlbv2(Qtr3dBinReader &binReader);
    bool parseJunkv2(Qtr3dBinReader &binReader);
    void parseJsonJunk(const QByteArray &data);
    void parseBinaryJunk(const QByteArray &data);

    void createNode(const QVariantMap &nodeInfo, Qtr3dModel::Node *parent = nullptr);
    void createMesh(const QVariantMap &meshInfo, int meshIndex);

    void splitAccessors(const QVariantList &bufferInfos);
    bool takeNextAccessor(const QVariantMap &bufferInfo);

    QByteArray  bufferView(int index) const;
    QVariantMap accessorInfo(int index) const;

    Qtr3dMesh   *loadTexturedMesh(const QVariantMap &positionInfo, const QVariantMap &faceInfo, const QVariantMap &normalInfo, const QVariantMap &textCoordInfo, const QImage &texture);
    Qtr3dMesh   *loadColoredMesh(const QVariantMap &positionInfo, const QVariantMap &faceInfo, const QVariantMap &normalInfo);
    QList<QVector3D> loadVectors(int componentType,  int offset, int count, const QByteArray &buffer) const;
    QList<int>       loadFaceIndexes(int componentType, int offset, int count, const QByteArray &buffer) const;
    QList<QPointF>   loadTextureCoords(int componentType, int offset, int count, const QByteArray &buffer) const;
    QImage           loadTexture(int materialIndex, int &texCoord) const;

    Qtr3dModel                 *mModel;

    // GLB v2 Parts
    QVariantMap        mJsonStruct; // Original JSON-Struct
    QList<QByteArray>  mAccessors;  // Original GLB-BinaryBuffer

    // Parsed mAccessors -> vertices, textures,...
    QList<QByteArray>  mBufferViews; // split from mAccessors


    // Parser Helper (to make algorithm faster)
    QMap<int,QList<int> > mJsonMeshes2qtrmeshes;
};

#endif // QTR3DGLBLOADER_H
