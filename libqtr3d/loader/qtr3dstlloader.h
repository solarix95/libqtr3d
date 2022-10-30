#ifndef QTR3DSTLLOADER_H
#define QTR3DSTLLOADER_H

#include <QString>
#include <QFile>
#include <QStringList>
#include "qtr3dmodelloader.h"

class Qtr3dStlLoader : public Qtr3dModelLoader
{
public:
    static bool supportsFile(const QString &filename);
    static bool loadFile(Qtr3dModel &model, const QString &filename, Qtr3dGeometryBufferFactory &factory);

    Qtr3dStlLoader();
    virtual ~Qtr3dStlLoader();

    virtual bool loadModel(Qtr3dModel &model, const QString &filename, Qtr3dGeometryBufferFactory &factory);

protected:

private:
    bool fromASCII(Qtr3dVertexMesh &mesh, QFile &f);
    bool fromBinary(Qtr3dVertexMesh &mesh, QFile &f);
    static QVector3D vectorFromStringList(const QStringList &strings);

    Qtr3dVertexMesh *mMesh;
};

#endif // QTR3DSTLLOADER_H
