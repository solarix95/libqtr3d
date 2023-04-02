#ifndef VIEWERFORM_H
#define VIEWERFORM_H

#include <QWidget>
#include <QList>

namespace Ui {
class ViewerForm;
}

class Qtr3dModel;
class Qtr3dGeometryState;
class Qtr3dCameraMovement;
class Qtr3dGeometry;

class ViewerForm : public QWidget
{
    Q_OBJECT

public:
    explicit ViewerForm(QWidget *parent = nullptr);
    ~ViewerForm();

private slots:
    void updateLight();
    void updateLightPos();
    void updateLightType();
    void updateLightAttrib();
    void updateModelPos();
    void selectLightColor();
    void selectFogColor();

private:
    Ui::ViewerForm *ui;

    QList<Qtr3dGeometry*>   mMeshes;
    Qtr3dGeometryState *mFloorState;
    Qtr3dGeometryState *mCubeState;
    Qtr3dGeometryState *mSphereState1;
    Qtr3dGeometryState *mSphereState2;
    Qtr3dGeometryState *mSphereState3;
    Qtr3dGeometryState *mLightState;

};

#endif // VIEWERFORM_H
