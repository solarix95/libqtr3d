#ifndef VIEWERFORM_H
#define VIEWERFORM_H

#include <QWidget>
#include <QVector3D>

namespace Ui {
class ViewerForm;
}

class Qtr3dModel;
class Qtr3dGeometryBufferState;
class Qtr3dCameraMovement;

class ViewerForm : public QWidget
{
    Q_OBJECT

public:
    explicit ViewerForm(QWidget *parent = nullptr);
    ~ViewerForm();

private:
    void setupSolarSystem();
    void setupStarDatabase();

    static double angleFromTime(const QString &time, bool firstIsGrad);

    Ui::ViewerForm *ui;

    Qtr3dModel               *mModel;
    Qtr3dGeometryBufferState *mModelState;
    Qtr3dCameraMovement      *mCameraMove;
    QVector3D                 mPolar;
};

#endif // VIEWERFORM_H
