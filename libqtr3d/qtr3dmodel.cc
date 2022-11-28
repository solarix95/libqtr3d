#include "qtr3dmodel.h"

//-------------------------------------------------------------------------------------------------
Qtr3dModel::Qtr3dModel(Qtr3dTextureFactory *textures)
    : Qtr3dGeometryBuffer()
    , mTexturesFactory(textures)
{
    Q_ASSERT(mTexturesFactory);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dModel::addGeometry(Qtr3dGeometryBuffer *buffer)
{
    Q_ASSERT(buffer);
    mModelBuffers << buffer;
    buffer->setParentBuffer(this);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dModel::registerBufferState(Qtr3dGeometryBufferState *s)
{
    for (auto *buffer : mModelBuffers)
        buffer->registerBufferState(s);
    Qtr3dGeometryBuffer::registerBufferState(s);
}

//-------------------------------------------------------------------------------------------------
QVector3D Qtr3dModel::center() const
{
    return mModelBuffers.isEmpty() ? QVector3D() : mModelBuffers.first()->center();
}

//-------------------------------------------------------------------------------------------------
double Qtr3dModel::radius() const
{
    return mModelBuffers.isEmpty() ? 0 : mModelBuffers.first()->radius();
}

//-------------------------------------------------------------------------------------------------
int Qtr3dModel::bufferCount() const
{
    return mModelBuffers.count();
}

//-------------------------------------------------------------------------------------------------
const Qtr3dGeometryBuffer *Qtr3dModel::buffer(int index) const
{
    Q_ASSERT(index >= 0);
    Q_ASSERT(index < mModelBuffers.count());
    return mModelBuffers[index];
}

//-------------------------------------------------------------------------------------------------
Qtr3dTextureFactory *Qtr3dModel::texturesFactory()
{
    return mTexturesFactory;
}
