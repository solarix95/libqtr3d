#ifndef QTR3DMODELANIMATION_H
#define QTR3DMODELANIMATION_H

#include <QObject>
#include <QVector3D>
#include <QQuaternion>
#include <QList>
#include <QMap>
#include <QStringList>
#include <QMatrix4x4>

class Qtr3dModelAnimation : public QObject
{
public:
    // TODO: Template-Type?
    struct PositionKey {
        float     mTime;
        QVector3D mValue;

        PositionKey(float t, const QVector3D &v) : mTime(t), mValue(v) {}
    };
    struct RotationKey {
        float       mTime;
        QQuaternion mValue;
        RotationKey(float t, const QQuaternion &v) : mTime(t), mValue(v) {}
    };
    struct ScaleKey {
        float     mTime;
        QVector3D mValue;
        ScaleKey(float t, const QVector3D &v) : mTime(t), mValue(v) {}
    };

    struct Channel {
        QString nodeName;
        QList<PositionKey> mPositionKeys;
        QList<RotationKey> mRotationKeys;
        QList<ScaleKey>    mScaleKeys;
    };

    Qtr3dModelAnimation(const QString &name, float duration, int ticksPerSec);

    QString name() const;

    // logical time: real duration is duration()/ticksPerSec()
    float   duration() const;
    int     ticksPerSec() const;

    void    addChannel(const Channel &channel);

    QStringList animatedNodes() const;
    QMatrix4x4  tranformNode(const QString &nodeName, float time) const;
    QMatrix4x4  tranformNode(const QString &nodeName, int   keyFrame) const;

private:

    const QMatrix4x4     &transformChannel(const Channel &channel, int keyFrame, QMatrix4x4 &matrix) const;
    QString               mName;
    float                 mDuration;
    int                   mTicksPerSec;
    QMap<QString,Channel> mChannels;
};

#endif // QTR3DMODELANIMATION_H
