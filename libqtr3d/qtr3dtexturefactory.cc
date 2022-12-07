#include <QDebug>
#include "qtr3dtypes.h"
#include "qtr3dtexturefactory.h"

//-------------------------------------------------------------------------------------------------
Qtr3dTextureFactory::Qtr3dTextureFactory()
{
}

//-------------------------------------------------------------------------------------------------
Qtr3dTextureFactory::~Qtr3dTextureFactory()
{
    clear();
}

//-------------------------------------------------------------------------------------------------
Qtr3dTexture *Qtr3dTextureFactory::texture(const QString &name)
{
    Qtr3dTexture *tex = mTextureDb.value(name,nullptr);
    if (!tex) {
        tex = createTexture(name);
        if (tex)
            mTextureDb[name] = tex;
    }
    return tex;
}

//-------------------------------------------------------------------------------------------------
Qtr3dTexture *Qtr3dTextureFactory::texture(const QImage &img, const QString &name)
{
    Qtr3dTexture *tex = mTextureDb.value(name,nullptr);
    if (!tex) {
        tex = createTexture(img);
        if (tex && !name.isEmpty())
            mTextureDb[name] = tex;
    }
    return tex;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dTextureFactory::clear()
{
    qDeleteAll(mTextureDb);
}

//-------------------------------------------------------------------------------------------------
Qtr3dTexture *Qtr3dTextureFactory::createTexture(const QString &name)
{
    QImage img(name);
    if (img.isNull())
        return nullptr;

    return createTexture(img);
}

//-------------------------------------------------------------------------------------------------
Qtr3dTexture *Qtr3dTextureFactory::createTexture(const QImage &img)
{
    Qtr3dTexture *tex = new Qtr3dTexture(img.mirrored());
    tex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    tex->setMagnificationFilter(QOpenGLTexture::Linear);

    qDebug() << tex->format();
    return tex;
}
