#ifndef QTR3D3DSLOADER_H
#define QTR3D3DSLOADER_H

#include <QString>
#include <QFile>
#include <QStringList>
#include <QList>
#include "qtr3dmodelloader.h"

class Qtr3dBinReader;

class Qtr3d3dsLoader : public Qtr3dModelLoader
{
public:
    static bool supportsFile(const QString &filename);
    static bool loadFile(Qtr3dVertexMesh &mesh, const QString &filename);

    Qtr3d3dsLoader();
    virtual ~Qtr3d3dsLoader();

    virtual bool loadMesh(Qtr3dVertexMesh &mesh, const QString &filename);

protected:

private:
    QList<quint16> processJunk(Qtr3dBinReader &reader, const QList<quint16> &contexts);

    void processMainJunk(Qtr3dBinReader &reader);       // Junk 0x4D4D
    void processEditorJunk(Qtr3dBinReader &reader);     // Junk 0x3D3D
    void processObjectJunk(Qtr3dBinReader &reader);     // Junk 0x4000
    void processMeshJunk(Qtr3dBinReader &reader);       // Junk 0x4100
    void processVerticesJunk(Qtr3dBinReader &reader);   // Junk 0x4110
    bool toJunk(quint16 junkIdent, Qtr3dBinReader &reader);
};

#endif // QTR3D3DSLOADER_H
