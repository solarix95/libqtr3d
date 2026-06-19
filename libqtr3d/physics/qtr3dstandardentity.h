#ifndef QTR3DSTANDARDENTITY_H
#define QTR3DSTANDARDENTITY_H

#include "qtr3dmotionbody.h"
#include "qtr3dabstractspace.h"

class Qtr3dStandardEntity : public Qtr3dMotionBody
{
    Q_OBJECT
public:
    Qtr3dStandardEntity(Qtr3dGeometryState *state, QVector3D pos, QVector3D lookAt = {0,0,0}, QVector3D orientation = {0,1,0});
    virtual ~Qtr3dStandardEntity();

    inline void setSpace(Qtr3dAbstractSpace *space) { setWorld(space); }
    inline Qtr3dAbstractSpace *space() const { return dynamic_cast<Qtr3dAbstractSpace*>(world()); }
};

#endif // QTR3DSTANDARDENTITY_H
