
#include <QtGlobal>
#include <QPushButton>
#include <QRadioButton>
#include <QCursor>
#include <QFileDialog>
#include <QSlider>
#include <QPushButton>
#include <QRadioButton>
#include <QDoubleSpinBox>
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
    , mCubeState(nullptr)
    , mSphereState(nullptr)
    , mLightState(nullptr)
{
    ui->setupUi(this);

    QObject::connect(ui->viewer, &Qtr3dWidget::initialized, [&]() {
        auto *cubeMesh = ui->viewer->createVertexMesh()->startMesh(Qtr3dGeometryBuffer::Quad, Qtr3dGeometryBuffer::CounterClockWise);

        // roof
        cubeMesh->addVertex({-1,1, 1},{0,1,0}, Qt::red);
        cubeMesh->addVertex({ 1,1, 1},{0,1,0},Qt::green);
        cubeMesh->addVertex({ 1,1,-1},{0,1,0},Qt::blue);
        cubeMesh->addVertex({-1,1,-1},{0,1,0},Qt::white);

        // front
        cubeMesh->addVertex({-1, 1, -1},{0,0,-1},Qt::white);
        cubeMesh->addVertex({ 1, 1, -1},{0,0,-1},Qt::blue);
        cubeMesh->addVertex({ 1,-1, -1},{0,0,-1},Qt::green);
        cubeMesh->addVertex({-1,-1, -1},{0,0,-1},Qt::red);

        // left
        cubeMesh->addVertex({ 1,-1,  1},{1,0,0},Qt::red);
        cubeMesh->addVertex({ 1,-1, -1},{1,0,0},Qt::white);
        cubeMesh->addVertex({ 1, 1, -1},{1,0,0},Qt::blue);
        cubeMesh->addVertex({ 1, 1,  1},{1,0,0},Qt::green);

        // back
        cubeMesh->addVertex({-1,-1,  1},{0,0,1},Qt::white);
        cubeMesh->addVertex({ 1,-1,  1},{0,0,1},Qt::blue);
        cubeMesh->addVertex({ 1, 1,  1},{0,0,1},Qt::green);
        cubeMesh->addVertex({-1, 1,  1},{0,0,1},Qt::red);

        // right
        cubeMesh->addVertex({ -1, 1,  1},{-1,0,0},Qt::green);
        cubeMesh->addVertex({ -1, 1, -1},{-1,0,0},Qt::blue);
        cubeMesh->addVertex({ -1,-1, -1},{-1,0,0},Qt::white);
        cubeMesh->addVertex({ -1,-1,  1},{-1,0,0},Qt::red);

        // bottom
        cubeMesh->addVertex({-1,-1,-1},{0,-1,0},Qt::white);
        cubeMesh->addVertex({ 1,-1,-1},{0,-1,0},Qt::blue);
        cubeMesh->addVertex({ 1,-1, 1},{0,-1,0},Qt::green);
        cubeMesh->addVertex({-1,-1, 1},{0,-1,0},Qt::red);

        cubeMesh->endMesh();

        mCubeState = ui->viewer->createBufferState(cubeMesh);
        mCubeState->setLightingType(Qtr3d::FlatLighting);

        // Create 2nd-Cube as LightPosition-Marker
        mLightState = ui->viewer->createBufferState(cubeMesh);
        mLightState->setLightingType(Qtr3d::NoLighting);
        mLightState->setScale({0.1,0.1,0.1});

        // Create Sphere
        auto *sphereMesh = ui->viewer->createVertexMesh();
        Qtr3dModelFactory::meshBySphere(*sphereMesh,60,QImage(":/pebbles_texture.jpg"));
        sphereMesh->setFaceOrientation(Qtr3dGeometryBuffer::CounterClockWise);
        mSphereState = ui->viewer->createBufferState(sphereMesh);

        // Create Floor
        auto *floor = ui->viewer->createTexturedMesh(":/pebbles_texture.jpg");
        floor->startMesh(Qtr3dGeometryBuffer::Triangle);
        floor->addQuad({-1,0,1},{1,0,1},{1,0,-1},{-1,0,-1},{0,1,0});
        floor->endMesh();
        floor->setFaceOrientation(Qtr3dGeometryBuffer::CounterClockWise);

        mFloorState = ui->viewer->createBufferState(floor);
        mFloorState->setScale({20,1,20});

        ui->viewer->camera()->lookAt({0,10,10},{0,0,0},{0,1,0});

        new Qtr3dFreeCameraController(ui->viewer);

        updateLightPos();
        updateLightType();
        updateModelPos();
        updateLightAttrib();

        // Scene complete... now activate the User-Interface
        connect(ui->sliderLight,  &QSlider::valueChanged, this, &ViewerForm::updateLightPos);
        connect(ui->sliderModels, &QSlider::valueChanged, this, &ViewerForm::updateModelPos);
        connect(ui->btnFlat,  &QRadioButton::toggled, this, &ViewerForm::updateLightType);
        connect(ui->btnPhong, &QRadioButton::toggled, this, &ViewerForm::updateLightType);
        connect(ui->btnFlat,  &QRadioButton::toggled, this, &ViewerForm::updateLightType);

        connect(ui->sldAmbient, &QSlider::valueChanged, this, [&](){ ui->ambientTot->setValue(ui->sldAmbient->value()/1000.0); });
        connect(ui->sldDiffuse, &QSlider::valueChanged, this, [&](){ ui->diffuseTot->setValue(ui->sldDiffuse->value()/1000.0); });
        connect(ui->sldSpecular,&QSlider::valueChanged, this, [&](){ ui->specularTot->setValue(ui->sldSpecular->value()/1000.0); });

        connect(ui->ambientTot,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                this, [&](){ ui->ambientX->setValue(ui->ambientTot->value());
                             ui->ambientY->setValue(ui->ambientTot->value());
                             ui->ambientZ->setValue(ui->ambientTot->value());});

        connect(ui->diffuseTot,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                this, [&](){ ui->diffuseX->setValue(ui->diffuseTot->value());
                             ui->diffuseY->setValue(ui->diffuseTot->value());
                             ui->diffuseZ->setValue(ui->diffuseTot->value());});

        connect(ui->specularTot,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                this, [&](){ ui->specularX->setValue(ui->specularTot->value());
                             ui->specularY->setValue(ui->specularTot->value());
                             ui->specularZ->setValue(ui->specularTot->value());});


        connect(ui->ambientX,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                this, &ViewerForm::updateLightAttrib);
        connect(ui->ambientY,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                this, &ViewerForm::updateLightAttrib);
        connect(ui->ambientZ,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                this, &ViewerForm::updateLightAttrib);

        connect(ui->diffuseX,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                this, &ViewerForm::updateLightAttrib);
        connect(ui->diffuseY,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                this, &ViewerForm::updateLightAttrib);
        connect(ui->diffuseZ,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                this, &ViewerForm::updateLightAttrib);

        connect(ui->specularX,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                this, &ViewerForm::updateLightAttrib);
        connect(ui->specularY,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                this, &ViewerForm::updateLightAttrib);
        connect(ui->specularZ,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                this, &ViewerForm::updateLightAttrib);



    });
}

//-------------------------------------------------------------------------------------------------
ViewerForm::~ViewerForm()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::updateLight()
{

    update();
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::updateLightPos()
{
    QVector3D startPos = {5,5,5};
    QMatrix4x4 turnMatrix;
    turnMatrix.rotate(ui->sliderLight->value(),{0,1,0});

    ui->viewer->primaryLightSource()->setPos(turnMatrix * startPos);
    mLightState->setPos(turnMatrix * startPos);
    ui->viewer->update();
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::updateLightType()
{
    Qtr3d::LightingType nextType;
    if (ui->btnNoLight->isChecked())
        nextType = Qtr3d::NoLighting;
    else if (ui->btnFlat->isChecked())
        nextType = Qtr3d::FlatLighting;
    else if (ui->btnPhong->isChecked())
        nextType = Qtr3d::PhongLighting;
    else {
        ui->btnFlat->setChecked(true);
        updateLightType();
        return;
    }
    mCubeState->setLightingType(nextType);
    mSphereState->setLightingType(nextType);
    mFloorState->setLightingType(nextType);

    ui->viewer->update();
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::updateLightAttrib()
{
    ui->viewer->primaryLightSource()->setAmbientStrength({float(ui->ambientX->value()), float(ui->ambientY->value()), float(ui->ambientZ->value())});
    ui->viewer->primaryLightSource()->setDiffuseStrength({float(ui->diffuseX->value()), float(ui->diffuseY->value()), float(ui->diffuseZ->value())});
    ui->viewer->primaryLightSource()->setSpecularStrength({float(ui->specularX->value()), float(ui->specularY->value()), float(ui->specularZ->value())});
    ui->viewer->update();
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::updateModelPos()
{
    QVector3D startPos;
    QMatrix4x4 turnMatrix;
    turnMatrix.rotate(ui->sliderModels->value(),{0,1,0});

    startPos = {3,1,0};
    mCubeState->setPos(turnMatrix * startPos);

    startPos = {-3,1,0};
    mSphereState->setPos(turnMatrix * startPos);

    ui->viewer->update();
}

//-------------------------------------------------------------------------------------------------
 /*
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
   */
