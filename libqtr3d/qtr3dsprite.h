#ifndef QTR3DSPRITE_H
#define QTR3DSPRITE_H

#include <QObject>
#include <QList>
#include "qtr3dtypes.h"

class Qtr3dCamera;
class Qtr3dGeometryBufferState;
class Qtr3dSprite : public QObject
{
public:
    Qtr3dSprite(Qtr3dGeometryBufferState *state, QList<Qtr3dTexture*> textures, Qtr3dCamera *camera = nullptr, bool billboard = true);

    void setTexture(int index); // Texture-Animation

private:
    Qtr3dGeometryBufferState *mState;
    QList<Qtr3dTexture*>      mTextures;
    int                       mTextureIndex;
};

#endif // QTR3DSPRITE_H