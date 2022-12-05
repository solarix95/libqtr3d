#ifndef SOLARSYSTEM_H
#define SOLARSYSTEM_H

#include <QObject>

class Qtr3dGeometryBufferFactory;
class SolarSystem : public QObject
{
    Q_OBJECT
public:
    SolarSystem(Qtr3dGeometryBufferFactory *factory);

private slots:
    void updateSolarSystem();

private:

};

#endif // SOLARSYSTEM_H
