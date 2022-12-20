#ifndef QTR3D3DSLOADER_H
#define QTR3D3DSLOADER_H

#include <functional>
#include <QString>
#include <QFile>
#include <QStringList>
#include <QList>
#include <QMap>
#include "qtr3dmodelloader.h"

class Qtr3dBinReader;

class Qtr3d3dsLoader : public Qtr3dModelLoader
{
public:
    static bool supportsFile(const QString &filename);
    static bool loadFile(Qtr3dModel &model, const QString &filename, Qtr3dGeometryBufferFactory &factory);

    Qtr3d3dsLoader();
    virtual ~Qtr3d3dsLoader();

    virtual bool loadModel(Qtr3dModel &model, const QString &filename, Qtr3dGeometryBufferFactory &factory);

protected:

private:
    qint32 processNextJunk(Qtr3dBinReader &reader, const QString &parentURL);
    void processNextJunks(Qtr3dBinReader &reader, const QString &parentURL, int junkSizes);
    void processObjectJunk(Qtr3dBinReader &reader,  const QString &parentURL, int junkSizes);       // Junk 0x4000
    void processMaterialNameJunk(Qtr3dBinReader &reader);                                           // Junk 0xAFFF-A000
    void processVerticesJunk(Qtr3dBinReader &reader);                                               // Junk 0x4110
    void processFacesJunk(Qtr3dBinReader &reader, const QString &parentURL, int junkSizes);         // Junk 0x4120
    void processMaterialJunk(Qtr3dBinReader &reader);                                               // Junk 0x4130
    void processMappingCoordJunk(Qtr3dBinReader &reader);                                           // Junk 0x4140

    QMap<QString,std::function<void(const QString &, int)> > mParserHooks;

    void setupMesh(const QString &fname);
    Qtr3dModel                *mModel;
    Qtr3dMesh                 *mMesh;

    QList<QVector3D>           mObjectVertices;
    QList<QList<int> >         mObjectFaces;
    QMap<QString,QList<int> >  mFaceIndexesByMaterial;
    QList<QString>             mMaterialNames;

    QList<QPointF>             mTextureMappings; // List< UV >
};

#endif // QTR3D3DSLOADER_H
