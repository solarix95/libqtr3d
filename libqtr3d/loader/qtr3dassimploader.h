#ifndef QTR3DASSIMPLOADER_H
#define QTR3DASSIMPLOADER_H

#include <QString>
#include <QStringList>
#include <QColor>
#include <QVector3D>
#include "qtr3dmodelloader.h"

class Qtr3dAssimpLoader : public Qtr3dModelLoader
{
public:    
    static bool supportsFile(const QString &filename);
    static bool loadFile(Qtr3dModel &model, const QString &filename,Options opts = DefaultOptions);

    Qtr3dAssimpLoader();
    virtual ~Qtr3dAssimpLoader();

    virtual bool loadModel(Qtr3dModel &model, const QString &filename, Options opt = DefaultOptions);
};

#endif // QTR3DASSIMPLOADER_H
