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

    QVector3D pos() const;
    QColor    color() const;
    QVector3D colorf() const;
    QVector3D ambientColor() const;
    QVector3D strengthAmbient() const;   // Support: Qtr3d::NoLighting + FlatLighting + PhongLighting

public slots:
    void      setColor(QColor c);
    void      setPos(const QVector3D &p);
    void      setAmbientStrength(float kAmbient);
    void      setAmbientStrength(const QVector3D &ambient);

signals:
    void changed(Qtr3dLightSource *light);

private:
    QVector3D mPos;
    QColor    mColor;

    QVector3D mAmbientFactor;
};

#endif // QTR3DLIGHTSOURCE_H
