#include <QJsonDocument>
#include <QJsonParseError>
#include <QVariantList>
#include <QVector3D>
#include <QDebug>
#include "qtr3djsonloader.h"
#include "../qtr3dfactory.h"
#include "../qtr3dassets.h"
#include "../qtr3dmesh.h"
#include "../qtr3dmodel.h"

//-------------------------------------------------------------------------------------------------
bool Qtr3dJsonLoader::supportsFile(const QString &filename)
{
    return filename.toLower().endsWith(".json");
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dJsonLoader::loadFile(Qtr3dModel &model, const QString &filename)
{
    Qtr3dJsonLoader loader;
    return loader.loadModel(model, fileContent(filename, -1), filename);
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dJsonLoader::loadFile(Qtr3dModel &model, const QByteArray &fileContent)
{
    Qtr3dJsonLoader loader;
    return loader.loadModel(model, fileContent);
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dJsonLoader::loadFile(Qtr3dModel &model, const QVariantMap &jsonModel)
{
    Qtr3dJsonLoader loader;
    return loader.loadModel(model, jsonModel);
}

//-------------------------------------------------------------------------------------------------
Qtr3dJsonLoader::Qtr3dJsonLoader()
{
}
Qtr3dJsonLoader::~Qtr3dJsonLoader() = default;

//-------------------------------------------------------------------------------------------------
QVector3D Qtr3dJsonLoader::jsonVector3(const QVariantMap &map, const QString &name, const QVector3D &fallback)
{
    QVariantList values = map.value(name).toList();
    if (values.count() != 3)
        return fallback;

    return {values[0].toFloat(), values[1].toFloat(), values[2].toFloat()};
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dJsonLoader::loadModel(Qtr3dModel &model, const QByteArray &fileContent, const QString &sourceFilenameOrPath)
{
    if (fileContent.isEmpty())
        return returnError("Empty JSON model file");

    QJsonParseError parseError;
    QVariantMap root = QJsonDocument::fromJson(fileContent, &parseError).toVariant().toMap();
    if (parseError.error != QJsonParseError::NoError) {
        const QString error = QString("JSON parser error: %1").arg(parseError.errorString());
        qWarning() << "Qtr3dJsonLoader:" << error;
        return returnError(error);
    }

    return loadModel(model, root, sourceFilenameOrPath);
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dJsonLoader::loadModel(Qtr3dModel &model, const QVariantMap &jsonModel, const QString &sourceFilenameOrPath)
{
    QVariantMap modelMap = jsonModel.value("model").toMap();
    if (modelMap.isEmpty() && jsonModel.contains("parts"))
        modelMap = jsonModel;

    QVariantList parts = modelMap.value("parts").toList();
    if (parts.isEmpty())
        return returnError("No model.parts in JSON model");

    int loadedParts = 0;
    for (const QVariant &partValue: parts) {
        QVariantMap part = partValue.toMap();
        QVariantMap meshMap = part.value("mesh").toMap();
        if (meshMap.isEmpty()) {
            qWarning() << "Qtr3dJsonLoader: part without mesh" << part.value("name").toString();
            continue;
        }

        auto *mesh = model.context()->createMesh(false);
        QVariantMap meshRoot;
        meshRoot["mesh"] = meshMap;
        if (!Qtr3d::meshByJson(*mesh, meshRoot, sourceFilenameOrPath)) {
            qWarning() << "Qtr3dJsonLoader: unable to build mesh" << part.value("name").toString();
            delete mesh;
            continue;
        }

        QVector3D pos = jsonVector3(part, "pos", {0,0,0});
        QVector3D rot = jsonVector3(part, "rot", {0,0,0});
        QVector3D scale = jsonVector3(part, "scale", {1,1,1});
        QString name = part.value("name", QString("part-%1").arg(loadedParts)).toString();

        model.createNode(mesh, pos, rot, scale, name);
        loadedParts++;
    }

    if (loadedParts <= 0)
        return returnError("No valid JSON model parts loaded");

    return true;
}
