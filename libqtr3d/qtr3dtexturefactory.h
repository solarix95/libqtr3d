#ifndef QTR3DTEXTUREFACTORY_H
#define QTR3DTEXTUREFACTORY_H

#include <QObject>
#include <QMap>
#include <QImage>
#include "qtr3dtypes.h"

class Qtr3dTextureFactory : public QObject
{
public:
    Qtr3dTextureFactory();
    virtual ~Qtr3dTextureFactory();

    Qtr3dTexture *texture(const QString &name);
    Qtr3dTexture *texture(const QImage &img, const QString &name = "");
    void          clear();

protected:
    virtual Qtr3dTexture *createTexture(const QString &name);
    virtual Qtr3dTexture *createTexture(const QImage &img);

private:
    QMap<QString, Qtr3dTexture*> mTextureDb;
};

#endif // QTR3DTEXTUREFACTORY_H
