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
    struct NodeTransform {
        QVector3D   position;
        QQuaternion rotation;
        QVector3D   scale;

        NodeTransform()
            : position()
            , rotation(1.0f, 0.0f, 0.0f, 0.0f)
            , scale(1.0f, 1.0f, 1.0f)
        {}
    };

    // TODO: Template-Type?
    struct PositionKey {
        float     mTime;
        QVector3D mValue;

        PositionKey(float t, const QVector3D &v) : mTime(t), mValue(v) {}
        PositionKey() : mTime(-1), mValue() {}
        inline bool isValid() const  { return mTime >= 0; }
    };
    struct RotationKey {
        float       mTime;
        QQuaternion mValue;
        RotationKey(float t, const QQuaternion &v) : mTime(t), mValue(v) {}
        RotationKey() : mTime(-1), mValue() {}
        inline bool isValid() const  { return mTime >= 0; }
    };
    struct ScaleKey {
        float     mTime;
        QVector3D mValue;
        ScaleKey(float t, const QVector3D &v) : mTime(t), mValue(v) {}
        ScaleKey() : mTime(-1), mValue() {}
        inline bool isValid() const  { return mTime >= 0; }
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
    // The first playback starts at zero; subsequent cycles restart here.
    float   loopStart() const;
    void    setLoopStart(float time);

    void    addChannel(const Channel &channel);

    QStringList animatedNodes() const;
    bool        nodeTransform(const QString &nodeName, float time, NodeTransform &transform) const;
    bool        tranformNode(const QString &nodeName, float time, QMatrix4x4 &matrix) const;

private:
    QString               mName;
    float                 mDuration;
    int                   mTicksPerSec;
    float                 mLoopStart;
    QMap<QString,Channel> mChannels;
};

#endif // QTR3DMODELANIMATION_H
