#include "qtr3dtexturedmesh.h"
#include "qtr3dmesh.h"
#include "qtr3dmodel.h"
#include "qtr3dtexturedmeshshader.h"
#include "qtr3dvertexmeshshader.h"

#include "qtr3dgeometrybufferfactory.h"

//-------------------------------------------------------------------------------------------------
Qtr3dGeometryBufferFactory::Qtr3dGeometryBufferFactory()
 : QObject()
 , mSimpleShader(nullptr)
 , mTextureShader(nullptr)
 , mDefaultColor(Qt::white)
{
}

Qtr3dGeometryBufferFactory::~Qtr3dGeometryBufferFactory() = default;

//-------------------------------------------------------------------------------------------------
void Qtr3dGeometryBufferFactory::init(Qtr3dVertexMeshShader *shader1, Qtr3dTexturedShader *shader2, Qtr3dTextureFactory *textures)
{
    mSimpleShader  = shader1;
    mTextureShader = shader2;
    mTextures      = textures;

    Q_ASSERT(mSimpleShader && mTextureShader && mTextures);
}

//-------------------------------------------------------------------------------------------------
Qtr3dTextureFactory *Qtr3dGeometryBufferFactory::textures()
{
    return mTextures;
}

//-------------------------------------------------------------------------------------------------
Qtr3dTexturedMesh *Qtr3dGeometryBufferFactory::createTexturedMesh(const QString &textureName)
{
   return nullptr;
}

//-------------------------------------------------------------------------------------------------
Qtr3dMesh *Qtr3dGeometryBufferFactory::createMesh()
{
    //Qtr3dMesh *mesh = new Qtr3dMesh(this);
    //mSimpleShader->registerBuffer(*mesh);
    //mesh->setDefaultColor(mDefaultColor);
    return nullptr;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dGeometryBufferFactory::setDefaultColor(QColor c)
{
    mDefaultColor = c;
}

