#include <QFile>
#include "qtr3dmodelloader.h"

//-------------------------------------------------------------------------------------------------
Qtr3dModelLoader::~Qtr3dModelLoader()
{
}

//-------------------------------------------------------------------------------------------------
QByteArray Qtr3dModelLoader::fileHeader(const QString &filename, int byteCount)
{
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly))
        return QByteArray();
    return f.read(byteCount);
}

