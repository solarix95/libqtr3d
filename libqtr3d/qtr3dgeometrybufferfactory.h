#ifndef QTR3DGEOMETRYBUFFERFACTORY_H
#define QTR3DGEOMETRYBUFFERFACTORY_H

#include <QObject>
#include <QColor>

class Qtr3dTexturedMesh;
class Qtr3dTexturedMeshShader;
class Qtr3dVertexMesh;
class Qtr3dVertexMeshShader;
class Qtr3dGeometryBuffer;
class Qtr3dTextureFactory;
class Qtr3dModel;

class Qtr3dGeometryBufferFactory : public QObject
{
public:
    Qtr3dGeometryBufferFactory();
    virtual ~Qtr3dGeometryBufferFactory();

    void init(Qtr3dVertexMeshShader *shader1, Qtr3dTexturedMeshShader *shader2, Qtr3dTextureFactory *textures);

    Qtr3dTextureFactory *textures();
    Qtr3dTexturedMesh   *createTexturedMesh(const QString &textureName="");
    Qtr3dVertexMesh     *createVertexMesh();
    Qtr3dModel          *createModel();
    void                 setDefaultColor(QColor c);

private:
    Qtr3dVertexMeshShader   *mSimpleShader;
    Qtr3dTexturedMeshShader *mTextureShader;
    Qtr3dTextureFactory     *mTextures;

    QColor                   mDefaultColor;

};

#endif // QTR3DGEOMETRYBUFFERFACTORY_H
