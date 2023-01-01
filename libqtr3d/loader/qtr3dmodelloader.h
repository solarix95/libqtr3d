#ifndef QTR3DMODELLOADER_H
#define QTR3DMODELLOADER_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include "../qtr3dmesh.h"
#include "../qtr3dmodel.h"
#include "../qtr3dgeometrybufferfactory.h"

class Qtr3dModelLoader : public QObject
{
public:
    enum Option {
           DefaultOptions = 0x0,
           VertexPoints  = 0x1,
           NormalVectors = 0x2
       };
       Q_DECLARE_FLAGS(Options, Option)

    using QObject::QObject;
    virtual ~Qtr3dModelLoader();

    QString lastError() const;

    static QByteArray fileHeader(const QString &filename, int byteCount = 100);
    static QString    addPath(const QString &sourceFIle, const QString &targetFile);
    static bool       isValidExternalTexture(const QString &textureFile);

protected:
    void setErrorMsg(const QString &errorMsg);
    bool returnError(const QString &errorMsg);
private:

    QString mLastError;
};

#endif // QTR3DMODELLOADER_H
