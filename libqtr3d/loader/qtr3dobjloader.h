#ifndef QTR3DOBJLOADER_H
#define QTR3DOBJLOADER_H

#include <QString>

#include "qtr3dmodelloader.h"

class Qtr3dObjLoader : public Qtr3dModelLoader
{
public:
    static bool supportsFile(const QString &filename);
    static bool loadFile(Qtr3dVertexMesh &mesh, const QString &filename);

    Qtr3dObjLoader();
    virtual ~Qtr3dObjLoader();

    virtual bool loadMesh(Qtr3dVertexMesh &mesh, const QString &filename);

protected:
    virtual void processObject(const QStringList &args);
    virtual void processVertex(const QStringList &args);
    virtual void processNormal(const QStringList &args);
    virtual void processFace(const QStringList &args);
    virtual void processSmoothshading(const QStringList &args);

private:
    Qtr3dVertexMesh *mMesh;
};

#endif // QTR3DOBJLOADER_H
