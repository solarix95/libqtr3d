#ifndef QTR3DBILLBOARD_H
#define QTR3DBILLBOARD_H

#include <QObject>

/*
   Qtr3dBillboard: turns the visible front always to the camera
*/

class Qtr3dGeometryState;
class Qtr3dCamera;

class Qtr3dBillboard : public QObject
{
public:
    Qtr3dBillboard(Qtr3dCamera *camera, Qtr3dGeometryState *state, QObject *parent = nullptr);

private slots:
    void updateState();
    void stateDeleted(QObject *state);

private:
    float alphaX() const;
    float alphaY() const;
    float alphaZ() const;

    Qtr3dCamera              *mCamera;
    Qtr3dGeometryState *mState;
};

#endif // QTR3DBILLBOARD_H
