#ifndef LIB_QTR3D_WIDGET
#define LIB_QTR3D_WIDGET

#include <QOpenGLWidget>
#include "qtr3dtypes.h"

class Qtr3dTexturedMesh;
class Qtr3dTexturedShader;
class Qtr3dMesh;
class Qtr3dVertexMeshShader;
class Qtr3dPlainShader;
class Qtr3dGeometry;
class Qtr3dGeometryState;
class Qtr3dTextureFactory;
class Qtr3dCamera;
class Qtr3dModel;
class Qtr3dLightSource;
class Qtr3dGeometryBufferFactory;
class Qtr3dAssets;
class Qtr3dShader;

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
    virtual ~Qtr3dWidget();

    void                        setOptions(Options ops);
    void                        setDefaultLighting(Qtr3d::LightingType t);
    Qtr3dCamera                *camera();
    Qtr3dTextureFactory        *textures();
    Qtr3dAssets                *assets();
    Qtr3dLightSource           *primaryLightSource();

    // Factories
    virtual Qtr3dMesh            *createMesh();
    virtual Qtr3dModel           *createModel();
    virtual Qtr3dGeometryState   *createState(Qtr3dGeometry *buffer, Qtr3d::LightingType ltype = Qtr3d::DefaultLighting);

public slots:
    void updateRequested();

protected:
    // Subclassing
    virtual void preparePainting();
    virtual void paint3D();
    virtual void paint2D();

    // Qt/Opengl
    virtual void initializeGL() override;
    virtual void paintGL() override;
    virtual void paintEvent(QPaintEvent *e)  override;
    virtual void resizeEvent(QResizeEvent *e) override;

    // Subclassing of Factories:
    virtual Qtr3dCamera         *createCamera();
    virtual Qtr3dTextureFactory *createTextureFactory();

signals:
    void initialized();

private:
    void preInitializing();
    void paintMeshes();
    void paintModels();
    void renderAnimatedModelMesh(Qtr3dShader *shader, const Qtr3dMesh *mesh, Qtr3dGeometryState *state, const Qtr3dCamera *camera, Qtr3d::LightingType lighting, const Qtr3dLightSource *light, const Qtr3dEnvironment &env);

    Options                  mOptions;
    Qtr3dCamera             *mCamera;
    Qtr3dTextureFactory     *mTextures;
    Qtr3dLightSource        *mLightSource;

    Qtr3dAssets            *mContext;

    Qtr3dPlainShader        *mPlainShader;
    Qtr3dVertexMeshShader   *mVertexMeshShader;
    Qtr3dTexturedShader     *mTexturedMeshShader;
};

#endif
