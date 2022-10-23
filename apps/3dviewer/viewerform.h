#ifndef VIEWERFORM_H
#define VIEWERFORM_H

#include <QWidget>

namespace Ui {
class ViewerForm;
}

class Qtr3dVertexMesh;
class Qtr3dGeometryBufferState;
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

private:

    Ui::ViewerForm *ui;

    Qtr3dVertexMesh          *mModel;
    Qtr3dGeometryBufferState *mModelState;
    Qtr3dCameraMovement      *mCameraMove;
};

#endif // VIEWERFORM_H
