
#include "shaders/qtr3dshader.h"
#include "qtr3dpointcloud.h"

//-------------------------------------------------------------------------------------------------
Qtr3dPointCloud::Qtr3dPointCloud(Qtr3dAssets *parent)
 : Qtr3dGeometry(PointCloudPipeline,parent)
 , mVerticesBufferId(0)
 , mElementBufferId(0)
 , mColorBufferId(0)
 , mVertexCount(0)
{
}

//-------------------------------------------------------------------------------------------------
Qtr3dPointCloud::~Qtr3dPointCloud()
{
    reset();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dPointCloud::reset()
{
    Qtr3dShader::releaseBO(mVerticesBufferId);
    Qtr3dShader::releaseBO(mElementBufferId);
}

//-------------------------------------------------------------------------------------------------
Qtr3dPointCloud *Qtr3dPointCloud::startCloud()
{
    reset();
    mVertices.reserve(150000000);
    return this;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dPointCloud::endCloud(bool doTrim)
{
    for (int i=0; i<mVertices.count(); i++) {
        mIndexes << (GLuint)i;
    }

    // Transfer Modeldata to Graphic-Memory
    mVerticesBufferId  = Qtr3dShader::makeBO(mVertices.data(),mVertices.count() * sizeof(Qtr3dPcVertex));
    mElementBufferId   = Qtr3dShader::makeBO(mIndexes.data(),mIndexes.count() * sizeof(GLuint), GL_ELEMENT_ARRAY_BUFFER);
    mVertexCount       = mVertices.count(); // ready to trim "mIndexes"

    if (mColors.count() > 0) {
        Q_ASSERT(mColors.count() == mVertices.count());
        mColorBufferId = Qtr3dShader::makeBO(mColors.data(),mColors.count() * sizeof(Qtr3dVector));
    }

    if (doTrim)
        trim();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dPointCloud::trim()
{
    mVertices.clear();
    mColors.clear();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dPointCloud::addVertex(const QVector3D &v)
{
    addVertex(v,material().ambient().mcolor);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dPointCloud::addVertex(const QVector3D &v, const QColor &c)
{
    mVertices << Qtr3dPcVertex(v);
    mColors   << Qtr3dVector(c.redF(), c.greenF(), c.blueF());
    analyze(v); // update bounding box
}

//-------------------------------------------------------------------------------------------------
int Qtr3dPointCloud::verticesCount() const
{
    return mVertices.count();
}

//-------------------------------------------------------------------------------------------------
const Qtr3dPcVertex &Qtr3dPointCloud::vertex(int i) const
{
    Q_ASSERT(i >= 0);
    Q_ASSERT(i < mVertices.count());
    return mVertices[i];
}

//-------------------------------------------------------------------------------------------------
GLenum Qtr3dPointCloud::bufferType() const
{
    return GL_POINTS;
}
