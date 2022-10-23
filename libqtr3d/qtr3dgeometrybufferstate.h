#ifndef QTR3DGEOMETRYBUFFERSTATE_H
#define QTR3DGEOMETRYBUFFERSTATE_H

#include <QObject>
#include <QMatrix4x4>
#include <QList>

class Qtr3dGeometryBuffer;

//-------------------------------------------------------------------------------------------------
class Qtr3dGeometryBufferState : public QObject
{
public:

    Qtr3dGeometryBufferState(bool isFlat = true);
    Qtr3dGeometryBufferState(const QVector3D &pos, const QVector3D &rotation);

    void setParent(Qtr3dGeometryBufferState *state);
    void setFlat(bool flat);

    inline const QVector3D &pos() const { return mPos; }
    inline const QVector3D &rot() const { return mRot; }

    void move(const QVector3D &pos);
    void move(const QVector3D &pos, const QVector3D &rotation);
    void setState(const QVector3D &pos, const QVector3D &rotation, const QVector3D &scale = {1,1,1});

    inline      bool        isFlat() const    { return mIsFlat;    }
    inline const QMatrix4x4 modelView() const { return mParent ? (mParent->modelView() * mModelView) : mModelView; }
    // inline const QMatrix4x4 &modelView() const { return mModelView; }

private:
    void updateMatrix();

    QMatrix4x4  mModelView;
    QVector3D   mPos;
    QVector3D   mRot;
    QVector3D   mScale;
    bool        mIsFlat;

    Qtr3dGeometryBufferState *mParent;
};

//-------------------------------------------------------------------------------------------------
typedef QList<Qtr3dGeometryBufferState*> Qtr3dGeometryBufferStates;
#endif // QTR3DGEOMETRYBUFFERSTATE_H
