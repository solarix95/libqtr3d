#ifndef QTR3DGEOMETRYBUFFER_H
#define QTR3DGEOMETRYBUFFER_H

#include <QObject>
#include <QList>
#include "qtr3dgeometrybufferstate.h"

class Qtr3dGeometryBufferState;

//-------------------------------------------------------------------------------------------------
class Qtr3dGeometryBuffer : public QObject
{
    Q_OBJECT
public:
    enum Type {
        Unknown,
        Dot,
        Line,
        Triangle,
        Quad
    };

    enum FaceOrientation {
        DefaultOrientation,
        ClockWise,
        CounterClockWise
    };

    Qtr3dGeometryBuffer();
    virtual ~Qtr3dGeometryBuffer();

    void            setFaceOrientation(FaceOrientation orientation);
    FaceOrientation faceOrientation() const;
    void            setDefaultColor(const QColor &c);
    Qtr3d::Material &material();
    const Qtr3d::Material &cMaterial() const;
    void            setBlendingEnabled(bool enabled);
    bool            blending() const;

    void                 setParentBuffer(Qtr3dGeometryBuffer *buffer);
    Qtr3dGeometryBuffer *parentBuffer() const;

    virtual QVector3D minValues() const; // lowest xyz
    virtual QVector3D maxValues() const; // max xyz
    virtual QVector3D center() const;
    virtual double    radius() const;

    // BufferState Iteration
    virtual void registerBufferState(Qtr3dGeometryBufferState *s);
    inline const Qtr3dGeometryBufferStates &bufferStates() const { return mBufferStates; }
    inline QColor defaultColor() { return mDefaultColor; }

protected:
    void analyze(const QVector3D &v);

private slots:
    void stateDestroyed(QObject *state);

private:
    FaceOrientation             mFaceOrientation;
    bool                        mBlending;
    Qtr3dGeometryBuffer        *mParent;
    Qtr3dGeometryBufferStates   mBufferStates;

    Qtr3d::Material             mMaterial;
    QColor                      mDefaultColor;

    QVector3D                   mMin;
    QVector3D                   mMax;
};

typedef QList<Qtr3dGeometryBuffer> Qtr3dGeometryBuffers;

#endif // QTR3DGEOMETRYBUFFER_H
