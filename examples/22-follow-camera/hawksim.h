#ifndef HAWKSIM_H
#define HAWKSIM_H

#include <QObject>
#include <QEvent>

class Qtr3dAssets;
class Qtr3dCamera;
class Qtr3dStandardEntity;

class HawkSim : public QObject
{
public:
    HawkSim();

    void init(Qtr3dAssets *context, Qtr3dCamera *camera);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:

    Qtr3dCamera         *mCamera;
    Qtr3dStandardEntity *mHawk;
};

#endif // HAWKSIM_H
