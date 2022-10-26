#ifndef QTR3DMODEL_H
#define QTR3DMODEL_H

#include <QObject>
#include "qtr3dgeometrybuffer.h"

class Qtr3dTextureFactory;
class Qtr3dModel : public Qtr3dGeometryBuffer
{
    Q_OBJECT
public:
    explicit Qtr3dModel(Qtr3dTextureFactory *textures);

    virtual void addGeometry(Qtr3dGeometryBuffer *buffer);
    virtual void registerBufferState(Qtr3dGeometryBufferState *s);

    QVector3D center() const;
    double    radius() const;

    Qtr3dTextureFactory *texturesFactory();

private:
    QList<Qtr3dGeometryBuffer*> mModelBuffers;
    Qtr3dTextureFactory        *mTexturesFactory;
};

#endif // QTR3DMODEL_H
