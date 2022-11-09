#ifndef QTR3DLIGHTSOURCE_H
#define QTR3DLIGHTSOURCE_H

#include <QObject>
#include <QColor>
#include <QVector3D>

class Qtr3dLightSource : public QObject
{
    Q_OBJECT
public:
    Qtr3dLightSource(QObject *parent = nullptr);
    Qtr3dLightSource(const QVector3D &p, QColor color, QObject *parent = nullptr);
    virtual ~Qtr3dLightSource();

    void      setPos(const QVector3D &p);
    QVector3D pos() const;

    void      setColor(QColor c);
    QColor    color() const;

    void      setStrength(float ambient, float diffuse, float specular);

private:
    QVector3D mPos;
    QColor    mColor;

    QVector3D mAmbientFactor;
    QVector3D mDiffuseFactor;
    QVector3D mSpecularFactor;
};

#endif // QTR3DLIGHTSOURCE_H
