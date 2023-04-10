#include "qtr3dsprite.h"
#include "qtr3dbillboard.h"
#include "../qtr3dcamera.h"
#include "../qtr3dgeometrystate.h"

//-------------------------------------------------------------------------------------------------
Qtr3dSprite::Qtr3dSprite(Qtr3dGeometryState *state, QList<Qtr3dTexture *> textures, Qtr3dCamera *camera, bool billboard)
 : QObject(state)
 , mState(state)
 , mTextures(textures)
 , mTextureIndex(-1)
{
    Q_ASSERT(mState);
    if (billboard && camera)
        new Qtr3dBillboard(camera,mState,this);
}

//-------------------------------------------------------------------------------------------------
void Qtr3dSprite::setTexture(int index)
{
    if (mTextureIndex == index)
        return;
    if (index >= mTextures.count()) {
        mTextureIndex = -1;
        return;
    }

    mTextureIndex = index;
    if (mTextureIndex < 0)
        return;

    Q_ASSERT(mTextureIndex >= 0 && mTextureIndex < mTextures.count());
}
