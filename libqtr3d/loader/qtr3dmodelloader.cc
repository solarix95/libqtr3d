#include <QFile>
#include <QFileInfo>
#include <QDir>
#include "qtr3dmodelloader.h"

//-------------------------------------------------------------------------------------------------
Qtr3dModelLoader::~Qtr3dModelLoader()
{
}

//-------------------------------------------------------------------------------------------------
QString Qtr3dModelLoader::lastError() const
{
    return mLastError;
}

//-------------------------------------------------------------------------------------------------
QByteArray Qtr3dModelLoader::fileHeader(const QString &filename, int byteCount)
{
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly))
        return QByteArray();
    return f.read(byteCount);
}

//-------------------------------------------------------------------------------------------------
QString Qtr3dModelLoader::addPath(const QString &sourceFile, const QString &targetFile)
{
    QFileInfo targetInfo(targetFile);
    if (targetInfo.isAbsolute())
        return targetFile;

    if (QFileInfo::exists(targetFile))
        return targetFile;

    QFileInfo sourceInfo(sourceFile);
    return sourceInfo.absoluteDir().absolutePath() + QDir::separator() + targetFile;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dModelLoader::isValidExternalTexture(const QString &textureFile)
{
    if (!QFileInfo::exists(textureFile))
        return false;
    QImage img(textureFile);
    return img.isNull() ? false : true;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dModelLoader::setErrorMsg(const QString &errorMsg)
{
    mLastError = errorMsg;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dModelLoader::returnError(const QString &errorMsg)
{
    mLastError = errorMsg;
    return mLastError.isEmpty();
}
