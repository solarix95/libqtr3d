
#include <QPushButton>
#include <QRadioButton>
#include <QCursor>
#include <QFileDialog>
#include <QStandardPaths>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dvertexmesh.h>
#include <libqtr3d/qtr3dcameracycler.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dmodelfactory.h>

#include "viewerform.h"
#include "ui_viewerform.h"

//-------------------------------------------------------------------------------------------------
ViewerForm::ViewerForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ViewerForm)
    , mModel(nullptr)
    , mModelState(nullptr)
    , mCameraMove(nullptr)
{
    ui->setupUi(this);
    ui->btnLoad->setEnabled(false);

    QObject::connect(ui->viewer, &Qtr3dWidget::initialized, [&]() {
        ui->btnLoad->setEnabled(true);
    });

    connect(ui->btnLoad, &QPushButton::clicked, this, &ViewerForm::load);
    connect(ui->btnCCW, &QRadioButton::clicked, this, &ViewerForm::updateVertexOrientation);
    connect(ui->btnCW, &QRadioButton::clicked, this, &ViewerForm::updateVertexOrientation);
}

//-------------------------------------------------------------------------------------------------
ViewerForm::~ViewerForm()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::load()
{
    static QString fileName = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);

    fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), fileName, tr("3D Files (*.stl *.obj)"));

    if (fileName.isEmpty())
        return;

    if (mModel) {
        mModel->deleteLater();
        mModelState->deleteLater();
    }
    mModel = ui->viewer->createVertexMesh();
    mModel->setDefaultColor(Qt::white);


    QApplication::setOverrideCursor(Qt::WaitCursor);
    Qtr3dModelFactory::meshByFile(*mModel,fileName);
    QApplication::restoreOverrideCursor();
    if (mModel->vertexCount() <= 0) {
        delete mModel;
        mModel = nullptr;
    }

    updateVertexOrientation();
    mModelState =  ui->viewer->createBufferState(mModel);
    mModelState->setFlat(false);

    if (mCameraMove)
        mCameraMove->deleteLater();
    mCameraMove = new Qtr3dCameraCycler(ui->viewer->camera(),30,0.3,{0,(float)(mModel->radius()),(float)(1.3*mModel->radius())},mModel->center());
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::updateVertexOrientation()
{
    if (!mModel)
        return;
    mModel->setVertexOrientation(ui->btnCCW->isChecked() ? Qtr3dVertexMesh::CounterClockWise : Qtr3dVertexMesh::ClockWise);
}
