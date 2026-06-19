#ifndef VIEWERFORM_H
#define VIEWERFORM_H

#include <QWidget>
#include <QString>
#include <libqtr3d/math3d/qtr3ddblvector.h>

namespace Ui {
class ViewerForm;
}

class Qtr3dModel;
class Qtr3dGeometryState;
class Qtr3dCameraMovement;
class QLabel;
class QComboBox;

class ViewerForm : public QWidget
{
    Q_OBJECT

public:
    explicit ViewerForm(QWidget *parent = nullptr);
    ~ViewerForm();

private slots:
    void load();
    void updateVertexOrientation();
    void updateLight();
    void initAnimationUi();
    void selectAnimation();
    void fitCamera();
    void applyCameraPreset(int index);
    void saveScreenshot();

private:
    void loadFile(const QString &filename);
    void updateModelInfo();
    Qtr3dDblVector3D modelCenter() const;
    float modelViewRadius() const;

    Ui::ViewerForm *ui;

    Qtr3dModel               *mModel;
    Qtr3dGeometryState       *mModelState;
    Qtr3dCameraMovement      *mCameraMove;
    QLabel                   *mModelInfo;
    QComboBox                *mCameraPreset;
    QString                   mCurrentFile;
};

#endif // VIEWERFORM_H
