#ifndef QTR3DBILLBOARD_H
#define QTR3DBILLBOARD_H

#include <QObject>

class Qtr3dGeometryBufferState;
class Qtr3dCamera;

class Qtr3dBillboard : public QObject
{
public:
    Qtr3dBillboard(Qtr3dCamera *camera, Qtr3dGeometryBufferState *state, QObject *parent = nullptr);

private slots:
    void updateState();
    void stateDeleted(QObject *state);

private:
    float alphaX() const;
    float alphaY() const;
    float alphaZ() const;

    Qtr3dCamera              *mCamera;
    Qtr3dGeometryBufferState *mState;
};

#endif // QTR3DBILLBOARD_H
