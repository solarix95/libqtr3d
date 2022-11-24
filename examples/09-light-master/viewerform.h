#ifndef VIEWERFORM_H
#define VIEWERFORM_H

#include <QWidget>
#include <QList>

namespace Ui {
class ViewerForm;
}

class Qtr3dModel;
class Qtr3dGeometryBufferState;
class Qtr3dCameraMovement;
class Qtr3dGeometryBuffer;

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

private:
    Ui::ViewerForm *ui;

    QList<Qtr3dGeometryBuffer*>   mMeshes;
    Qtr3dGeometryBufferState *mFloorState;
    Qtr3dGeometryBufferState *mCubeState;
    Qtr3dGeometryBufferState *mSphereState;
    Qtr3dGeometryBufferState *mLightState;

};

#endif // VIEWERFORM_H
