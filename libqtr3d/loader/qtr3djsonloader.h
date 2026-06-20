#ifndef QTR3DJSONLOADER_H
#define QTR3DJSONLOADER_H

#include <QByteArray>
#include <QString>
#include <QVariantMap>
#include <QVector3D>
#include "qtr3dmodelloader.h"

class Qtr3dJsonLoader : public Qtr3dModelLoader
{
public:
    static bool supportsFile(const QString &filename);
    static bool loadFile(Qtr3dModel &model, const QString &filename);
    static bool loadFile(Qtr3dModel &model, const QByteArray &fileContent);
    static bool loadFile(Qtr3dModel &model, const QVariantMap &jsonModel);

    Qtr3dJsonLoader();
    virtual ~Qtr3dJsonLoader();

    virtual bool loadModel(Qtr3dModel &model, const QByteArray &fileContent, const QString &sourceFilenameOrPath = QString());
    virtual bool loadModel(Qtr3dModel &model, const QVariantMap &jsonModel, const QString &sourceFilenameOrPath = QString());

private:
    static QVector3D jsonVector3(const QVariantMap &map, const QString &name, const QVector3D &fallback);
};

#endif // QTR3DJSONLOADER_H
