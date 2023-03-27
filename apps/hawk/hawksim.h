#ifndef HAWKSIM_H
#define HAWKSIM_H

#include <QObject>

class Qtr3dContext;
class Qtr3dCamera;

class HawkSim
{
public:
    HawkSim();

    void init(Qtr3dContext *context, Qtr3dCamera *camera);

private:

    Qtr3dCamera *mCamera;
};

#endif // HAWKSIM_H
