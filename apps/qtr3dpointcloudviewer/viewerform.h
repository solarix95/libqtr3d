#ifndef VIEWERFORM_H
#define VIEWERFORM_H

#include <QWidget>

namespace Ui {
class ViewerForm;
}

class Qtr3dModel;
class Qtr3dGeometry;
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

private:
    void loadFile(const QString &filename);
    void loadEthSemantic3dTxt(const QString &filename);
    void loadPly(const QString &filename);
    void updateModelInfo(int vertexCount);

    Ui::ViewerForm *ui;

    Qtr3dGeometry            *mGeometry;
    Qtr3dModel               *mModel;
    Qtr3dGeometryState       *mModelState;
    Qtr3dCameraMovement      *mCameraMove;
};

#endif // VIEWERFORM_H
