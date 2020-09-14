#ifndef SOLARSYSTEM_H
#define SOLARSYSTEM_H

#include "libqtr3d/qtr3dmodel.h"

class Qtr3dWidget;
class SolarSystem : public Qtr3dModel
{
    Q_OBJECT
public:
    SolarSystem(Qtr3dWidget *parent);

private slots:
    void updateSolarSystem();

private:

};

#endif // SOLARSYSTEM_H
