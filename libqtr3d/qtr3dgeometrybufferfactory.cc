#include "qtr3dtexturedmesh.h"
#include "qtr3dvertexmesh.h"
#include "qtr3dmodel.h"
#include "qtr3dtexturedmeshshader.h"
#include "qtr3dvertexmeshshader.h"

#include "qtr3dgeometrybufferfactory.h"

//-------------------------------------------------------------------------------------------------
Qtr3dGeometryBufferFactory::Qtr3dGeometryBufferFactory()
 : QObject()
 , mSimpleShader(nullptr)
 , mTextureShader(nullptr)
{
}

Qtr3dGeometryBufferFactory::~Qtr3dGeometryBufferFactory() = default;

//-------------------------------------------------------------------------------------------------
void Qtr3dGeometryBufferFactory::init(Qtr3dVertexMeshShader *shader1, Qtr3dTexturedMeshShader *shader2, Qtr3dTextureFactory *textures)
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
    Qtr3dTexturedMesh *geometryBuffer = new Qtr3dTexturedMesh(textures(),textureName);
    mTextureShader->registerBuffer(*geometryBuffer);
    return geometryBuffer;
}

//-------------------------------------------------------------------------------------------------
Qtr3dVertexMesh *Qtr3dGeometryBufferFactory::createVertexMesh()
{
    Qtr3dVertexMesh *mesh = new Qtr3dVertexMesh();
    mSimpleShader->registerBuffer(*mesh);
    return mesh;
}

//-------------------------------------------------------------------------------------------------
Qtr3dModel *Qtr3dGeometryBufferFactory::createModel()
{
    return new Qtr3dModel(textures());
}

