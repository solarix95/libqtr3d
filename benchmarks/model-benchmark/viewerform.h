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

private:
    Ui::ViewerForm *ui;
};

#endif // VIEWERFORM_H
