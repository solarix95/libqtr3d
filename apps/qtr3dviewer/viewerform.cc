
#include <QPushButton>
#include <QRadioButton>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QCursor>
#include <QFileDialog>
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
    ui->viewer->setOptions(Qtr3dWidget::MSAA4);

    QObject::connect(ui->viewer, &Qtr3dWidget::initialized, [&]() {

        ui->btnLoad->setEnabled(true);
        connect(new Qtr3dFreeCameraController(ui->viewer), &Qtr3dFreeCameraController::positionChanged, ui->viewer->primaryLightSource(), &Qtr3dLightSource::setPos);
        for (auto arg: qApp->arguments()) {
            if (arg.startsWith("--load=")) {
                auto parts = arg.split("=");
                if (parts.count() == 2)
                    loadFile(parts[1]);
            }
        }

        ui->viewer->primaryLightSource()->setAmbientStrength(0.5);
        connect(ui->viewer->camera(), &Qtr3dCamera::positionChanged, ui->viewer->primaryLightSource(), &Qtr3dLightSource::setPos);


        auto *center = ui->viewer->createMesh();
        Qtr3d::meshByXyzAxis(*center, 5);
        ui->viewer->createState(center)->setLightingType(Qtr3d::NoLighting);

        ui->viewer->camera()->lookAt({5,5,5},{0,0,0},{0,1,0});
    });

    connect(ui->btnLoad, &QPushButton::clicked, this, &ViewerForm::load);
    connect(ui->btnCCW, &QRadioButton::clicked, this, &ViewerForm::updateVertexOrientation);
    connect(ui->btnCW, &QRadioButton::clicked, this, &ViewerForm::updateVertexOrientation);

    connect(ui->btnLightOn, &QRadioButton::clicked, this, &ViewerForm::updateLight);
    connect(ui->btnLightOff, &QRadioButton::clicked, this, &ViewerForm::updateLight);
    connect(ui->cbxAnimList, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this]() { selectAnimation(); });

    connect(ui->spbAnimCurrentKey, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [this]() { updateAnimator(); });
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
void ViewerForm::initAnimationUi()
{
    ui->cbxAnimList->clear();
    ui->spbAnimCurrentKey->setValue(0);
    ui->spbAnimTicks->setValue(0);
    ui->spbAnimTpS->setValue(0);

    if (!mModel || mModel->animations().isEmpty())
        return;


    ui->cbxAnimList->addItems(mModel->animations());
    selectAnimation();
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::selectAnimation()
{
    if (ui->cbxAnimList->currentIndex() < 0)
        return;
    if (!mModel || !mModelState)
        return;

    Qtr3dModelAnimation *anim = mModel->animationByName(ui->cbxAnimList->currentText());
    if (!anim)
        return;

    ui->spbAnimTicks->setValue(anim->duration());
    ui->spbAnimTpS->setValue(anim->ticksPerSec());
    ui->spbAnimCurrentKey->setValue(0);

    mModelState->setAnimator(new Qtr3dModelAnimator(anim));
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::updateAnimator()
{
    if (!mModelState || !mModelState->animator())
        return;

    mModelState->animator()->setKeyFrameIndex(ui->spbAnimCurrentKey->value());
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::loadFile(const QString &filename)
{
    if (mModel) {
        mModel->deleteLater();
        mModel = nullptr;
        mModelState = nullptr;
        initAnimationUi();
    }
    mModel = ui->viewer->createModel();

    QApplication::setOverrideCursor(Qt::WaitCursor);
    bool done = Qtr3d::modelByFile(*mModel,filename);
    QApplication::restoreOverrideCursor();

    if (!done)
        return;

    mModelState =  ui->viewer->createState(mModel);
    updateLight();
    updateVertexOrientation();

    qDebug() << mModel->radius() << (1/mModel->radius()) << mModelState->center();

    mModelState->setScale(1/mModel->radius());
    ui->viewer->camera()->lookAt(mModelState->center() + QVector3D({3,3,3}), mModelState->center(), {0,1,0});
    initAnimationUi();
}
