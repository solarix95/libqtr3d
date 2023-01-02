
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
#include <libqtr3d/qtr3dlightsource.h>
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
        ui->viewer->primaryLightSource()->setAmbientStrength(0.7);

        ui->btnLoad->setEnabled(true);
        connect(new Qtr3dFreeCameraController(ui->viewer), &Qtr3dFreeCameraController::positionChanged, ui->viewer->primaryLightSource(), &Qtr3dLightSource::setPos);
        for (auto arg: qApp->arguments()) {
            if (arg.startsWith("--load=")) {
                auto parts = arg.split("=");
                if (parts.count() == 2)
                    loadFile(parts[1]);
            }
        }

        ui->viewer->primaryLightSource()->setAmbientStrength(0.1);
        connect(ui->viewer->camera(), &Qtr3dCamera::positionChanged, ui->viewer->primaryLightSource(), &Qtr3dLightSource::setPos);

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
                                            tr("Open Image"), fileName, tr("3D Files (*.stl *.obj *.3ds *.glb *.ply *.lws *.lwo)"));

    if (fileName.isEmpty())
        return;

    loadFile(fileName);
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::updateVertexOrientation()
{
    if (!mModel)
        return;
    mModel->setFaceOrientation(ui->btnCCW->isChecked() ? Qtr3d::CounterClockWise : Qtr3d::ClockWise);
    ui->viewer->update();
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::updateLight()
{
    if (!mModelState)
        return;
    mModelState->setLightingType(ui->btnLightOff->isChecked() ? Qtr3d::NoLighting : Qtr3d::FlatLighting);//  setFlat(ui->btnLightOff->isChecked());
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
    Qtr3dModelFactory::modelByFile(*mModel,filename);
    QApplication::restoreOverrideCursor();

    mModelState =  ui->viewer->createState(mModel);
    updateLight();
    updateVertexOrientation();

    // qDebug() << mModel->radius() << mModel->center();
    ui->viewer->camera()->lookAt(mModel->center() + QVector3D(0.0,0.0, 2*mModel->radius() ), mModel->center(), {0,1,0});
}
