#ifndef QTR3DGEOMETRYBUFFERFACTORY_H
#define QTR3DGEOMETRYBUFFERFACTORY_H

#include <QObject>
#include <QColor>

class Qtr3dTexturedMesh;
class Qtr3dTexturedShader;
class Qtr3dMesh;
class Qtr3dVertexMeshShader;
class Qtr3dGeometryBuffer;
class Qtr3dTextureFactory;
class Qtr3dModel;

class Qtr3dGeometryBufferFactory : public QObject
{
public:
    Qtr3dGeometryBufferFactory();
    virtual ~Qtr3dGeometryBufferFactory();

    void init(Qtr3dVertexMeshShader *shader1, Qtr3dTexturedShader *shader2, Qtr3dTextureFactory *textures);

    Qtr3dTextureFactory *textures();
    Qtr3dTexturedMesh   *createTexturedMesh(const QString &textureName="");
    Qtr3dMesh           *createMesh();
    void                 setDefaultColor(QColor c);

private:
    Qtr3dVertexMeshShader   *mSimpleShader;
    Qtr3dTexturedShader *mTextureShader;
    Qtr3dTextureFactory     *mTextures;

    QColor                   mDefaultColor;

};

#endif // QTR3DGEOMETRYBUFFERFACTORY_H
