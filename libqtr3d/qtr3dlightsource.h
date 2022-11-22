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

    // 7x the same... just for easy use :)
    void      setStrength(float ambient, float diffuse, float specular);
    void      setAmbientStrength(float kAmbient);
    void      setDiffuseStrength(float kDiffuse);
    void      setSpecularStrength(float kSpecular);
    void      setAmbientStrength(const QVector3D &ambient);
    void      setDiffuseStrength(const QVector3D &diffuse);
    void      setSpecularStrength(const QVector3D &specular);

    QVector3D strengthAmbient() const;   // Support: Qtr3d::NoLighting + FlatLighting + PhongLighting
    QVector3D strengthDiffuse() const;   // Support: Qtr3d::FlatLighting + PhongLighting
    QVector3D strengthSpecular() const;  // Support: Qtr3d::PhongLighting

signals:
    void changed(Qtr3dLightSource *light);

private:
    QVector3D mPos;
    QColor    mColor;

    QVector3D mAmbientFactor;
    QVector3D mDiffuseFactor;
    QVector3D mSpecularFactor;
};

#endif // QTR3DLIGHTSOURCE_H
