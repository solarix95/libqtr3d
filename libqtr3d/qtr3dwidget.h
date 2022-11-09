#ifndef LIB_QTR3D_WIDGET
#define LIB_QTR3D_WIDGET

#include <QOpenGLWidget>
#include "qtr3dtypes.h"

class Qtr3dTexturedMesh;
class Qtr3dTexturedMeshShader;
class Qtr3dVertexMesh;
class Qtr3dVertexMeshShader;
class Qtr3dGeometryBuffer;
class Qtr3dGeometryBufferState;
class Qtr3dTextureFactory;
class Qtr3dCamera;
class Qtr3dModel;
class Qtr3dLightSource;
class Qtr3dGeometryBufferFactory;

//-------------------------------------------------------------------------------------------------
class Qtr3dWidget : public QOpenGLWidget
{
    Q_OBJECT
public:

    enum Option {
        NoOption,
        MSAA4   = 0x0001,
        MSAA16  = 0x0002,
    };
    Q_DECLARE_FLAGS(Options, Option)

    Qtr3dWidget(Options ops = NoOption, QWidget *parent = NULL);
    Qtr3dWidget(QWidget *parent);

    Qtr3dCamera                *camera();
    Qtr3dTextureFactory        *textures();
    Qtr3dGeometryBufferFactory *factory();
    Qtr3dLightSource           *primaryLightSource();

    // Factories

    virtual Qtr3dTexturedMesh        *createTexturedMesh(const QString &textureName="");
    virtual Qtr3dVertexMesh          *createVertexMesh();
    virtual Qtr3dModel               *createModel();
    virtual Qtr3dGeometryBufferState *createBufferState(Qtr3dGeometryBuffer *buffer, Qtr3d::LightingType ltype = Qtr3d::NoLighting);

public slots:
    void updateRequested();

protected:
    virtual void initializeGL();
    virtual void paintGL();
    virtual void resizeEvent(QResizeEvent *e);

    // Subclassing of Factories:
    virtual Qtr3dCamera         *createCamera();
    virtual Qtr3dTextureFactory *createTextureFactory();

signals:
    void initialized();

private:
    void initializeMultisampleAntiAliasing();

    Options                  mOptions;
    Qtr3dCamera             *mCamera;
    Qtr3dTextureFactory     *mTextures;
    Qtr3dGeometryBufferFactory *mFactory;
    Qtr3dLightSource        *mLightSource;

    Qtr3dVertexMeshShader   *mVertexMeshShader;
    Qtr3dTexturedMeshShader *mTexturedMeshShader;

    QColor                   mClearColor;
};

#endif
