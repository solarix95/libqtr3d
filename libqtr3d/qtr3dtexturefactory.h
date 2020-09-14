#ifndef QTR3DTEXTUREFACTORY_H
#define QTR3DTEXTUREFACTORY_H

#include <QObject>
#include <QMap>
#include "qtr3dtypes.h"

class Qtr3dTextureFactory : public QObject
{
public:
    Qtr3dTextureFactory();
    virtual ~Qtr3dTextureFactory();

    Qtr3dTexture *texture(const QString &name);
    void          clear();

protected:
    virtual Qtr3dTexture *createTexture(const QString &name);

private:
    QMap<QString, Qtr3dTexture*> mTextureDb;
};

#endif // QTR3DTEXTUREFACTORY_H
