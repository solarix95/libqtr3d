
#include <QPushButton>
#include <QRadioButton>
#include <QComboBox>
#include <QDateTime>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QCursor>
#include <QFileDialog>
#include <QFileInfo>
#include <QLabel>
#include <QMessageBox>
#include <QSizePolicy>
#include <QStandardPaths>
#include <QCoreApplication>

#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dmodel.h>
#include <libqtr3d/qtr3dmodelanimation.h>
#include <libqtr3d/qtr3dmodelanimator.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dlightsource.h>
#include <libqtr3d/qtr3dfactory.h>
#include <libqtr3d/extras/qtr3dfreecameracontroller.h>
#include <libqtr3d/physics/qtr3dfpsloop.h>
#include "viewerform.h"
#include "ui_viewerform.h"

//-------------------------------------------------------------------------------------------------
ViewerForm::ViewerForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ViewerForm)
    , mModel(nullptr)
    , mModelState(nullptr)
    , mCameraMove(nullptr)
    , mModelInfo(nullptr)
    , mCameraPreset(nullptr)
{
    ui->setupUi(this);

    mModelInfo = new QLabel(this);
    mModelInfo->setMinimumWidth(260);
    mModelInfo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    mCameraPreset = new QComboBox(this);
    mCameraPreset->addItems({tr("Fit"), tr("Front"), tr("Side"), tr("Top"), tr("Iso")});

    auto *btnFit = new QPushButton(tr("Fit"), this);
    auto *btnScreenshot = new QPushButton(tr("Screenshot"), this);

    ui->horizontalLayout->insertWidget(0, mModelInfo, 1);
    ui->horizontalLayout->insertWidget(1, mCameraPreset);
    ui->horizontalLayout->insertWidget(2, btnFit);
    ui->horizontalLayout->insertWidget(3, btnScreenshot);

    connect(btnFit, &QPushButton::clicked, this, &ViewerForm::fitCamera);
    connect(btnScreenshot, &QPushButton::clicked, this, &ViewerForm::saveScreenshot);
    connect(mCameraPreset, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, &ViewerForm::applyCameraPreset);

    updateModelInfo();
    ui->btnLoad->setEnabled(false);
    ui->viewer->setOptions((Qtr3dWidget::Options)Qtr3dWidget::MSAA4);
    ui->viewer->assets()->loop().setFps(30);

    QObject::connect(&ui->viewer->assets()->loop(), &Qtr3dFpsLoop::stepDone, ui->viewer,[&]() { ui->viewer->update();});
    QObject::connect(ui->viewer, &Qtr3dWidget::initialized, [&]() {

        ui->btnLoad->setEnabled(true);
        connect(ui->viewer->camera(), &Qtr3dCamera::positionChanged, ui->viewer->primaryLightSource(), &Qtr3dLightSource::setPos);
        ui->viewer->camera()->lookAt({5,5,5},{0,0,0},{0,1,0});

        connect(new Qtr3dFreeCameraController(ui->viewer), &Qtr3dFreeCameraController::positionChanged, ui->viewer->primaryLightSource(), &Qtr3dLightSource::setPos);
        for (auto arg: qApp->arguments()) {
            if (arg.startsWith("--load=")) {
                auto parts = arg.split("=");
                if (parts.count() == 2)
                    loadFile(parts[1]);
            }
        }

        ui->viewer->primaryLightSource()->setAmbientStrength(0.5);

        auto *center = ui->viewer->createMesh();
        Qtr3d::meshByXyzAxis(*center, 5);
        ui->viewer->createState(center)->setLightingType(Qtr3d::NoLighting);
    });

    connect(ui->btnLoad, &QPushButton::clicked, this, &ViewerForm::load);
    connect(ui->btnCCW, &QRadioButton::clicked, this, &ViewerForm::updateVertexOrientation);
    connect(ui->btnCW, &QRadioButton::clicked, this, &ViewerForm::updateVertexOrientation);
    connect(ui->btnAnimPlay, &QPushButton::clicked, this, [this]() {
       if (mModelState && mModelState->animator()) {
           mModelState->animator()->setLoop(ui->chkLoop->isChecked());
           mModelState->animator()->start();
       }
    });
    connect(ui->chkLoop, &QCheckBox::clicked, this, [this]() {
       if (mModelState && mModelState->animator())
           mModelState->animator()->setLoop(ui->chkLoop->isChecked());
    });

    connect(ui->spbAnimTpS, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [this]() {
       if (mModelState && mModelState->animator())
           mModelState->animator()->setTicksPerSecond(ui->spbAnimTpS->value());
    });

    connect(ui->btnLightOn, &QRadioButton::clicked, this, &ViewerForm::updateLight);
    connect(ui->btnLightOff, &QRadioButton::clicked, this, &ViewerForm::updateLight);
    connect(ui->cbxAnimList, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this]() { selectAnimation(); });

    updateVertexOrientation();
    updateLight();
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
                                            tr("Open Image"), fileName, tr("3D Files (*.stl *.obj *.3ds *.glb *.ply *.lws *.lwo *.fbx)"));

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
void ViewerForm::initAnimationUi()
{
    ui->cbxAnimList->clear();
    ui->spbAnimTicks->setValue(0);
    ui->spbAnimTpS->setValue(0);

    if (!mModel || mModel->animations().isEmpty()) {
        ui->playerGroup->setEnabled(false);
        return;
    }

    ui->playerGroup->setEnabled(true);
    ui->cbxAnimList->addItem(QString("no animation"));
    ui->cbxAnimList->addItems(mModel->animations());
    selectAnimation();
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::selectAnimation()
{
    if (!mModel || !mModelState)
        return;

    if (ui->cbxAnimList->currentIndex() < 1) {
        mModelState->setAnimator(nullptr);
        ui->spbAnimTicks->setValue(0);
        ui->spbAnimTpS->setValue(0);
        return;
    }

    Qtr3dModelAnimation *anim = mModel->animationByName(ui->cbxAnimList->currentText());
    if (!anim)
        return;

    ui->spbAnimTicks->setValue(anim->duration());
    ui->spbAnimTpS->setValue(anim->ticksPerSec());

    mModelState->setAnimator(new Qtr3dModelAnimator(anim));
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::loadFile(const QString &filename)
{
    if (mModel) {
        mModel->deleteLater();
        mModel = nullptr;
        mModelState = nullptr;
        mCurrentFile.clear();
        initAnimationUi();
        updateModelInfo();
    }
    mModel = ui->viewer->createModel();

    QApplication::setOverrideCursor(Qt::WaitCursor);
    bool done = Qtr3d::modelByFile(*mModel,filename);
    QApplication::restoreOverrideCursor();

    if (!done) {
        mModel->deleteLater();
        mModel = nullptr;
        mModelState = nullptr;
        mCurrentFile.clear();
        updateModelInfo();
        QMessageBox::warning(this, tr("Load failed"), tr("Could not load model:\n%1").arg(filename));
        return;
    }

    mCurrentFile = filename;

    mModelState =  ui->viewer->createState(mModel);
    updateLight();
    updateVertexOrientation();

    // qDebug() << mModel->radius() << (1/mModel->radius()) << mModelState->center();

    const double modelRadius = mModel->radius();
    mModelState->setScale(modelRadius > 0.001 ? float(1.0/modelRadius) : 1.0f);
    fitCamera();
    initAnimationUi();
    updateModelInfo();
}

//-------------------------------------------------------------------------------------------------
Qtr3dDblVector3D ViewerForm::modelCenter() const
{
    return mModelState ? mModelState->center() : Qtr3dDblVector3D(0,0,0);
}

//-------------------------------------------------------------------------------------------------
float ViewerForm::modelViewRadius() const
{
    if (!mModelState)
        return 1.0f;

    const float radius = mModelState->radius();
    return radius > 0.001f ? radius : 1.0f;
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::fitCamera()
{
    const Qtr3dDblVector3D center = modelCenter();
    const float distance = modelViewRadius() * 3.0f;
    ui->viewer->camera()->lookAt(center + Qtr3dDblVector3D(distance, distance, distance), center, {0,1,0});
    ui->viewer->update();
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::applyCameraPreset(int index)
{
    if (!mModelState)
        return;

    const Qtr3dDblVector3D center = modelCenter();
    const float distance = modelViewRadius() * 3.0f;

    switch (index) {
    case 0:
        fitCamera();
        return;
    case 1:
        ui->viewer->camera()->lookAt(center + Qtr3dDblVector3D(0,0,distance), center, {0,1,0});
        break;
    case 2:
        ui->viewer->camera()->lookAt(center + Qtr3dDblVector3D(distance,0,0), center, {0,1,0});
        break;
    case 3:
        ui->viewer->camera()->lookAt(center + Qtr3dDblVector3D(0,distance,0), center, {0,0,-1});
        break;
    case 4:
        ui->viewer->camera()->lookAt(center + Qtr3dDblVector3D(distance,distance,distance), center, {0,1,0});
        break;
    default:
        return;
    }

    ui->viewer->update();
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::saveScreenshot()
{
    QString dir = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    if (dir.isEmpty())
        dir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);

    QString base = QFileInfo(mCurrentFile).completeBaseName();
    if (base.isEmpty())
        base = QStringLiteral("qtr3dviewer");

    const QString defaultFile = dir + QLatin1Char('/') + base + QLatin1Char('-')
            + QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMdd-hhmmss")) + QStringLiteral(".png");

    const QString filename = QFileDialog::getSaveFileName(this, tr("Save Screenshot"), defaultFile, tr("PNG Images (*.png)"));
    if (filename.isEmpty())
        return;

    const QImage image = ui->viewer->grabFramebuffer();
    if (!image.save(filename))
        QMessageBox::warning(this, tr("Screenshot failed"), tr("Could not save screenshot:\n%1").arg(filename));
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::updateModelInfo()
{
    if (!mModel || !mModelState) {
        mModelInfo->setText(tr("No model loaded"));
        mModelInfo->setToolTip(QString());
        return;
    }

    const QFileInfo fileInfo(mCurrentFile);
    const QString fileName = fileInfo.fileName().isEmpty() ? tr("Model") : fileInfo.fileName();
    const Qtr3dDblVector3D center = mModelState->center();

    mModelInfo->setText(tr("%1 | meshes %2 | nodes %3 | anim %4 | r %5")
                        .arg(fileName)
                        .arg(mModel->meshes().count())
                        .arg(mModel->nodes().count())
                        .arg(mModel->animations().count())
                        .arg(modelViewRadius(), 0, 'f', 2));

    mModelInfo->setToolTip(tr("File: %1\nMeshes: %2\nNodes: %3\nAnimations: %4\nCenter: %5, %6, %7\nView radius: %8")
                           .arg(mCurrentFile)
                           .arg(mModel->meshes().count())
                           .arg(mModel->nodes().count())
                           .arg(mModel->animations().count())
                           .arg(center.x, 0, 'f', 3)
                           .arg(center.y, 0, 'f', 3)
                           .arg(center.z, 0, 'f', 3)
                           .arg(modelViewRadius(), 0, 'f', 3));
}

