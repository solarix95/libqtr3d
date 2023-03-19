
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
#include <QColorDialog>
#include <QCoreApplication>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dmodel.h>
#include <libqtr3d/qtr3dcameracycler.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dlightsource.h>
#include <libqtr3d/qtr3dmodelfactory.h>
#include <libqtr3d/debug/qtr3dfreecameracontroller.h>
#include "viewerform.h"
#include "ui_viewerform.h"

//-------------------------------------------------------------------------------------------------
ViewerForm::ViewerForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ViewerForm)
    , mCubeState(nullptr)
    , mSphereState1(nullptr)
    , mSphereState2(nullptr)
    , mSphereState3(nullptr)
    , mLightState(nullptr)
{
    ui->setupUi(this);

    QObject::connect(ui->viewer, &Qtr3dWidget::initialized, [&]() {
        auto *cubeMesh = ui->viewer->createMesh()->startMesh(Qtr3d::Quad, Qtr3d::CounterClockWise);

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
        mMeshes << cubeMesh;

        mCubeState = ui->viewer->createState(cubeMesh);

        // Create 2nd-Cube as LightPosition-Marker
        mLightState = ui->viewer->createState(cubeMesh, Qtr3d::NoLighting);
        mLightState->setScale({0.1,0.1,0.1});

        // Create Textured Sphere
        auto *sphereMesh1 = ui->viewer->createMesh();
        Qtr3dModelFactory::meshBySphere(*sphereMesh1,60,QImage(":/pebbles_texture.jpg"));
        sphereMesh1->setFaceOrientation(Qtr3d::CounterClockWise);
        mSphereState1 = ui->viewer->createState(sphereMesh1);
        mMeshes << sphereMesh1;

        auto *sphereMesh2 = ui->viewer->createMesh();
        Qtr3dModelFactory::meshBySphere(*sphereMesh2,60,Qt::black);
        sphereMesh2->setFaceOrientation(Qtr3d::CounterClockWise);
        mSphereState2 = ui->viewer->createState(sphereMesh2);
        mMeshes << sphereMesh2;

        auto *sphereMesh3 = ui->viewer->createMesh();
        Qtr3dModelFactory::meshBySphere(*sphereMesh3,60,Qt::white);
        sphereMesh3->setFaceOrientation(Qtr3d::CounterClockWise);
        mSphereState3 = ui->viewer->createState(sphereMesh3);
        mMeshes << sphereMesh2;

        // Create Floor
        auto *floor = ui->viewer->createMesh();
        floor->setTexture(QImage(":/pebbles_texture.jpg"));
        floor->startMesh(Qtr3d::Triangle);
        floor->addQuad({-1,0,1},{1,0,1},{1,0,-1},{-1,0,-1},{0,1,0});
        floor->endMesh();
        floor->setFaceOrientation(Qtr3d::CounterClockWise);
        mMeshes << floor;

        mFloorState = ui->viewer->createState(floor);
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

        connect(ui->sldFog, &QSlider::valueChanged, this, [&](){
            ui->viewer->bufferContext()->environment().setFogDistance(ui->sldFog->value());
            ui->viewer->update();});

        connect(ui->sldLightAmbient, &QSlider::valueChanged, this, [&](){ ui->lightambientTot->setValue(ui->sldLightAmbient->value()/1000.0); });
        connect(ui->sldAmbient, &QSlider::valueChanged, this, [&](){ ui->ambientTot->setValue(ui->sldAmbient->value()/1000.0); });
        connect(ui->sldDiffuse, &QSlider::valueChanged, this, [&](){ ui->diffuseTot->setValue(ui->sldDiffuse->value()/1000.0); });
        connect(ui->sldSpecular,&QSlider::valueChanged, this, [&](){ ui->specularTot->setValue(ui->sldSpecular->value()/1000.0); });
        connect(ui->sldShininess,&QSlider::valueChanged, this, [&](){ui->shininess->setValue(ui->sldShininess->value()/10); });

        connect(ui->lightambientTot,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                this, [&](){ ui->lightambientX->setValue(ui->lightambientTot->value());
                             ui->lightambientY->setValue(ui->lightambientTot->value());
                             ui->lightambientZ->setValue(ui->lightambientTot->value());});

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

        connect(ui->lightambientX,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                this, &ViewerForm::updateLightAttrib);
        connect(ui->lightambientY,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                this, &ViewerForm::updateLightAttrib);
        connect(ui->lightambientZ,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                this, &ViewerForm::updateLightAttrib);


        connect(ui->ambientX,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                this, &ViewerForm::updateLightAttrib);
        connect(ui->ambientY,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                this, &ViewerForm::updateLightAttrib);
        connect(ui->ambientZ,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
                this, &ViewerForm::updateLightAttrib);
        connect(ui->shininess,static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged),
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


        connect(ui->btnPickColor, &QPushButton::clicked, this, &ViewerForm::selectLightColor);
        connect(ui->btnPickFog,   &QPushButton::clicked, this, &ViewerForm::selectFogColor);


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

    ui->viewer->setDefaultLighting(nextType);
    ui->viewer->update();
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::updateLightAttrib()
{
    ui->viewer->primaryLightSource()->setAmbientStrength({float(ui->lightambientX->value()), float(ui->lightambientY->value()), float(ui->lightambientZ->value())});

    for (auto *mesh: mMeshes) {
        mesh->material().ambient().strength  = {float(ui->ambientX->value()),  float(ui->ambientY->value()),  float(ui->ambientZ->value())};
        mesh->material().diffuse().strength  = {float(ui->diffuseX->value()),  float(ui->diffuseY->value()),  float(ui->diffuseZ->value())};
        mesh->material().specular().strength = {float(ui->specularX->value()), float(ui->specularY->value()), float(ui->specularZ->value())};
        mesh->material().setShininess(ui->shininess->value());
    }

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

    turnMatrix.rotate(90,{0,1,0});
    mSphereState1->setPos(turnMatrix * startPos);
    turnMatrix.rotate(90,{0,1,0});
    mSphereState2->setPos(turnMatrix * startPos);
    turnMatrix.rotate(90,{0,1,0});
    mSphereState3->setPos(turnMatrix * startPos);

    ui->viewer->update();
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::selectLightColor()
{
    QColor c = QColorDialog::getColor(ui->viewer->primaryLightSource()->color(), this,"Ambient Color");
    ui->viewer->primaryLightSource()->setColor(c);
    ui->viewer->update();
    ui->edtColorCode->setText(c.name());
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::selectFogColor()
{
    QColor c = QColorDialog::getColor(ui->viewer->bufferContext()->environment().clearColor(), this,"Clear Color");
    ui->viewer->bufferContext()->environment().setClearColor(c);
    ui->viewer->update();
    ui->edtFogColor->setText(c.name());
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
