#ifndef VIEWERFORM_H
#define VIEWERFORM_H

#include <QWidget>
#include <QVector3D>

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
    void updateSolarSystem();

private:
    void setupSolarSystem();
    void setupStarDatabase();


    static double angleFromTime(const QString &time, bool firstIsGrad);

    Ui::ViewerForm *ui;

    Qtr3dModel               *mModel;
    Qtr3dGeometryState       *mModelState;
    Qtr3dGeometryState       *mIssState;

    Qtr3dCameraMovement      *mCameraMove;
    QVector3D                 mPolar;

    int                       mTimePlayerStep;
};

#endif // VIEWERFORM_H
