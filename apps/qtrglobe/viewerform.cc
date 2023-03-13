
#include <math.h>

#include <QPushButton>
#include <QRadioButton>
#include <QCursor>
#include <QFileDialog>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QJsonDocument>

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
    , mModel(nullptr)
    , mModelState(nullptr)
    , mCameraMove(nullptr)
{
    ui->setupUi(this);

    QObject::connect(ui->viewer, &Qtr3dWidget::initialized, [&]() {
        new Qtr3dFreeCameraController(ui->viewer);

        setupSolarSystem();
    });
}

//-------------------------------------------------------------------------------------------------
ViewerForm::~ViewerForm()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::setupSolarSystem()
{
    {
        auto *mesh = ui->viewer->createMesh();
        Qtr3dModelFactory::meshBySphere(*mesh,256,QImage(":/sun.jpg"));
         mesh->setFaceOrientation(Qtr3d::CounterClockWise);
        auto *state = ui->viewer->createState(mesh);
        state->setLightingType(Qtr3d::NoLighting);
        state->setRotation({90,0,0});
        state->setScale(1.0/mesh->radius());
    }

    // Globe
    // if (0)
    {
        mModel = ui->viewer->createModel();
        mModel->setFaceOrientation(Qtr3d::CounterClockWise);
        Qtr3dModelFactory::modelByFile(*mModel,":/Earth_1_12756.glb");
        mModelState =  ui->viewer->createState(mModel);
        mModelState->setPos({100,0,0});
        mModelState->setScale(1.0/mModel->radius());
    }

    // Vertical Axe
    // if (0)
    {
        auto *mesh = ui->viewer->createMesh();
        mesh->startMesh(Qtr3d::Line);
        mesh->addVertex({0.0,+2.0,0.0}, Qt::gray);
        mesh->addVertex({0.0,-2.0,0.0},Qt::gray);
        mesh->endMesh();
        auto *state = ui->viewer->createState(mesh);
        state->setPos({100,0,0});

    }


    // qDebug() << mModel->radius() << mModel->center();
    // ui->viewer->camera()->lookAt(mModel->center() + QVector3D(0.0,0.0, 2*mModel->radius() ), mModel->center(), {0,1,0});
    ui->viewer->camera()->lookAt(QVector3D(100-2*mModel->radius(),0,0), {100,0,0}, {0,1,0});

    ui->viewer->primaryLightSource()->setPos({0,0,0});
    ui->viewer->primaryLightSource()->setAmbientStrength(0);

    mModel->material().ambient().strength = {0,0,0};
    mModel->material().diffuse().strength = {1.1,1.1,1.1};

    mModelState->setLightingType(Qtr3d::FlatLighting);

    setupStarDatabase();
}

void ViewerForm::setupStarDatabase()
{

    QFile inputFile(":/bsc5p_min.json");
    inputFile.open(QIODevice::ReadOnly);
    if (!inputFile.isOpen())
        return;

    QVariantList stars =  QJsonDocument::fromJson(inputFile.readAll()).toVariant().toList();

    double minMag = 1; // 6;
    double maxMag = 8;

    QVariantMap map;
    QMatrix4x4  matrix;

    auto *mesh = ui->viewer->createMesh();
    mesh->startMesh(Qtr3d::Dot);

    for (auto starInfo : stars) {
        map = starInfo.toMap();
        if (!map.contains("galacticLongitude"))
            continue;


        double longitude = map.value("galacticLongitude").toDouble();
        double latitude  = map.value("galacticLatitude").toDouble();
        double magnitude = map.value("visualMagnitude").toDouble();
        if (magnitude < minMag)
            continue;

        double x = 1000 * cos((latitude/360.0)*2*3.1415) * cos((longitude/360.0)*2*3.1415);
        double y = 1000 * cos((latitude/360.0)*2*3.1415) * sin((longitude/360.0)*2*3.1415);
        double z = 1000 * sin((latitude/360.0)*2*3.1415);

        // minMag = qMin(minMag,magnitude);
        // maxMag = qMax(maxMag,magnitude);

        double expMin = exp(minMag);
        double expMax = exp(maxMag);
        double expMag = exp(qMin(maxMag,magnitude));

        float color = (expMag-expMin)/(expMax-expMin);
        mesh->addVertex(QVector3D(x,y,z),QColor(color*255,color*255,color*255));
    }

    mesh->endMesh(true);
    auto *state = ui->viewer->createState(mesh,Qtr3d::NoLighting);
}



