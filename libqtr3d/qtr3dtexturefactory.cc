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

    Qtr3dTexture *tex = new Qtr3dTexture(img);
    tex->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    tex->setMagnificationFilter(QOpenGLTexture::Linear);

    return tex;
}
