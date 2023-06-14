#ifndef VIEWERFORM_H
#define VIEWERFORM_H

#include <QWidget>

namespace Ui {
class ViewerForm;
}

class Qtr3dModel;
class Qtr3dGeometryState;
class Qtr3dCameraMovement;

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

private:
    void loadFile(const QString &filename);
    Ui::ViewerForm *ui;

    Qtr3dModel               *mModel;
    Qtr3dGeometryState       *mModelState;
    Qtr3dCameraMovement      *mCameraMove;
};

#endif // VIEWERFORM_H
