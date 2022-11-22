
#include <QPushButton>
#include <QRadioButton>
#include <QCursor>
#include <QFileDialog>
#include <QStandardPaths>
#include <QCoreApplication>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dmodel.h>
#include <libqtr3d/qtr3dcameracycler.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dmodelfactory.h>
#include <libqtr3d/qtr3dtexturedmesh.h>
#include <libqtr3d/debug/qtr3dfreecameracontroller.h>
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
        new Qtr3dFreeCameraController(ui->viewer);
        for (auto arg: qApp->arguments()) {
            if (arg.startsWith("--load=")) {
                auto parts = arg.split("=");
                if (parts.count() == 2)
                    loadFile(parts[1]);
            }
        }
    });

    connect(ui->btnLoad, &QPushButton::clicked, this, &ViewerForm::load);
    connect(ui->btnCCW, &QRadioButton::clicked, this, &ViewerForm::updateVertexOrientation);
    connect(ui->btnCW, &QRadioButton::clicked, this, &ViewerForm::updateVertexOrientation);

    connect(ui->btnLightOn, &QRadioButton::clicked, this, &ViewerForm::updateLight);
    connect(ui->btnLightOff, &QRadioButton::clicked, this, &ViewerForm::updateLight);
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
        tr("Open Image"), fileName, tr("3D Files (*.stl *.obj *.3ds *.glb)"));

    if (fileName.isEmpty())
        return;

    loadFile(fileName);
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::updateVertexOrientation()
{
    if (!mModel)
        return;
    mModel->setFaceOrientation(ui->btnCCW->isChecked() ? Qtr3dGeometryBuffer::CounterClockWise : Qtr3dGeometryBuffer::ClockWise);
    ui->viewer->update();
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::updateLight()
{
    if (!mModelState)
        return;
    mModelState->setLightingType(Qtr3d::FlatLighting);//  setFlat(ui->btnLightOff->isChecked());
    ui->viewer->update();
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::loadFile(const QString &filename)
{
    if (mModel) {
        mModel->deleteLater();
        mModelState = nullptr;
    }
    mModel = ui->viewer->createModel();

    QApplication::setOverrideCursor(Qt::WaitCursor);
    Qtr3dModelFactory::modelByFile(*mModel,filename, *ui->viewer->factory());
    QApplication::restoreOverrideCursor();

    mModelState =  ui->viewer->createBufferState(mModel);
    updateLight();
    updateVertexOrientation();

    qDebug() << mModel->radius() << mModel->center();
    ui->viewer->camera()->lookAt({0,0,-(float)(5*mModel->radius())}, mModel->center(), {0,1,0});
}
