#ifndef LIB_QTR3D_WIDGET
#define LIB_QTR3D_WIDGET

#include <QOpenGLWidget>

class Qtr3dTexturedQuad;
class Qtr3dTexturedQuadShader;
class Qtr3dVertexMesh;
class Qtr3dVertexMeshShader;
class Qtr3dGeometryBuffer;
class Qtr3dGeometryBufferState;
class Qtr3dTextureFactory;
class Qtr3dCamera;

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


    Qtr3dCamera         *camera();
    Qtr3dTextureFactory *textures();

    // Factories

    virtual Qtr3dTexturedQuad        *createTexturedQuad(const QString &textureName="");
    virtual Qtr3dVertexMesh          *createVertexMesh();
    virtual Qtr3dGeometryBufferState *createBufferState(Qtr3dGeometryBuffer *buffer, bool isFlat = true);

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

    Qtr3dVertexMeshShader   *mVertexMeshShader;
    Qtr3dTexturedQuadShader *mTexturedQuadShader;

    QColor                   mClearColor;
};

#endif
