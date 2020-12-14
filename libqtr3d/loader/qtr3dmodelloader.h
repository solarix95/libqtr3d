#ifndef QTR3DMODELLOADER_H
#define QTR3DMODELLOADER_H

#include <QObject>
#include "../qtr3dvertexmesh.h"

class Qtr3dModelLoader : public QObject
{
public:
    using QObject::QObject;
    virtual ~Qtr3dModelLoader();

    virtual bool loadMesh(Qtr3dVertexMesh &mesh, const QString &filename) = 0;
};

#endif // QTR3DMODELLOADER_H
