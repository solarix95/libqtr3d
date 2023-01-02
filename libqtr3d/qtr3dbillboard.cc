#include <cmath>
#include <QDebug>
#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>
#include "qtr3dbillboard.h"
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dgeometrybufferstate.h>

//-------------------------------------------------------------------------------------------------
Qtr3dBillboard::Qtr3dBillboard(Qtr3dCamera *camera, Qtr3dGeometryBufferState *state, QObject *parent)
 : QObject(parent)
 , mCamera(camera)
 , mState(state)
{
    Q_ASSERT(mCamera);
    Q_ASSERT(mState);

    connect(mState, &Qtr3dGeometryBufferState::destroyed, this, &Qtr3dBillboard::stateDeleted);
    connect(mCamera,&Qtr3dCamera::positionChanged, this, &Qtr3dBillboard::updateState);

    updateState();
}

//-------------------------------------------------------------------------------------------------
void Qtr3dBillboard::updateState()
{
    Q_ASSERT(mCamera);
    Q_ASSERT(mState);

    float alpha = alphaY();
    mState->setRotation({0,alpha,0});
}

//-------------------------------------------------------------------------------------------------
void Qtr3dBillboard::stateDeleted(QObject *state)
{
    if (state != mState)
        return;
    mState = nullptr;
    deleteLater();
}

//-------------------------------------------------------------------------------------------------
float Qtr3dBillboard::alphaX() const
{
    QVector2D v2 = { mCamera->pos().y() - mState->pos().y(),
                     mCamera->pos().z() - mState->pos().z()};

    bool turn180 = v2.y() < 0;
    QVector2D v1 = { (turn180 ? -1.0f : 1.0f), 0.0f};
    v2.normalize();

    float alpha= std::acos(v1.x()*v2.x() + v1.y()*v2.y());

    if ((v2.y()*v1.x() - v2.x()*v1.y()) > 0)
        alpha = (alpha/(2*3.1415))*360; // RAD to Grad

    qDebug() << (turn180 ? -90-alpha : 90-alpha);
    return turn180 ? -90-alpha : 90-alpha;
}

//-------------------------------------------------------------------------------------------------
float Qtr3dBillboard::alphaY() const
{
    QVector2D v2 = { mCamera->pos().x() - mState->pos().x(),
                     mCamera->pos().z() - mState->pos().z()};

    bool turn180 = v2.y() < 0;
    QVector2D v1 = { (turn180 ? -1.0f : 1.0f), 0.0f};
    v2.normalize();

    float alpha= std::acos(v1.x()*v2.x() + v1.y()*v2.y());

    if ((v2.y()*v1.x() - v2.x()*v1.y()) > 0)
        alpha = (alpha/(2*3.1415))*360; // RAD to Grad

    return turn180 ? -90-alpha : 90-alpha;
}

float Qtr3dBillboard::alphaZ() const
{
    QVector2D v2 = { mCamera->pos().x() - mState->pos().x(),
                     mCamera->pos().y() - mState->pos().y()};

    bool turn180 = v2.y() < 0;
    QVector2D v1 = { (turn180 ? -1.0f : 1.0f), 0.0f};
    v2.normalize();

    float alpha= std::acos(v1.x()*v2.x() + v1.y()*v2.y());

    if ((v2.y()*v1.x() - v2.x()*v1.y()) > 0)
        alpha = (alpha/(2*3.1415))*360; // RAD to Grad

    return turn180 ? -90-alpha : 90-alpha;
}
