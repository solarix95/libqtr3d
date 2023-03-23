
#include <math.h>

#include <QPushButton>
#include <QRadioButton>
#include <QCursor>
#include <QFileDialog>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QJsonDocument>
#include <QTimer>

#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dmodel.h>
#include <libqtr3d/qtr3dcameracycler.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dlightsource.h>
#include <libqtr3d/qtr3dfactory.h>
#include <libqtr3d/debug/qtr3dfreecameracontroller.h>
#include <libqtr3d/debug/qtr3dfollowcamera.h>
#include "viewerform.h"
#include "ui_viewerform.h"

#define EARTH_START_POSITION {100,0,0}

//-------------------------------------------------------------------------------------------------
ViewerForm::ViewerForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ViewerForm)
    , mModel(nullptr)
    , mModelState(nullptr)
    , mCameraMove(nullptr)
    , mTimePlayerStep(0)
{
    ui->setupUi(this);
    ui->viewer->setOptions(Qtr3dWidget::MSAA16);
    ui->viewer->setFocus();

    QObject::connect(ui->viewer, &Qtr3dWidget::initialized, [&]() {

        setupSolarSystem();

        ui->dateTimeEdit->setEnabled(true);
        ui->checkBox->setEnabled(true);
        ui->checkBox->setChecked(true);

        ui->btnFwd->setEnabled(true);
        ui->btnRev->setEnabled(true);
        ui->btnPause->setEnabled(true);

        connect(ui->btnPause, &QPushButton::clicked, [this]() {
            ui->checkBox->setChecked(false);
            mTimePlayerStep = 0;
        });

        connect(ui->btnFwd, &QPushButton::clicked, [this]() {
            ui->checkBox->setChecked(false);
            mTimePlayerStep = mTimePlayerStep > 0 ? mTimePlayerStep*10 : 1;
        });
        connect(ui->btnRev, &QPushButton::clicked, [this]() {
            ui->checkBox->setChecked(false);
            mTimePlayerStep = mTimePlayerStep < 0 ? mTimePlayerStep*10 : -1;
        });

        new Qtr3dFreeCameraController(ui->viewer);
        auto *follower = new Qtr3dFollowCamera(ui->viewer,mModelState);

        QTimer *t = new QTimer(ui->viewer);
        connect(t, &QTimer::timeout, this, [this, follower]() {
            if (ui->checkBox->isChecked()) {
                ui->dateTimeEdit->setDateTime(QDateTime::currentDateTimeUtc());
                mTimePlayerStep = 0;
            } else
                ui->dateTimeEdit->setDateTime(ui->dateTimeEdit->dateTime().addSecs(mTimePlayerStep*60));
            follower->process();
            updateSolarSystem();
        });
        t->start(1000/30);
    });

    connect(ui->dateTimeEdit, &QDateTimeEdit::dateTimeChanged, this, &ViewerForm::updateSolarSystem);
}

//-------------------------------------------------------------------------------------------------
ViewerForm::~ViewerForm()
{
    delete ui;
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::updateSolarSystem()
{
   if (!mModel)
       return;

   QDateTime utc            = ui->dateTimeEdit->dateTime();
   int   minuteOfDay        = utc.time().hour()*60 + utc.time().minute();
   QDateTime summerSolstice = QDateTime(QDate(utc.date().year(),6,21), QTime(12,00),Qt::UTC);
   int minutesToSolstice    = utc.msecsTo(summerSolstice)/1000/60;
   float yearAngle          = 360 * minutesToSolstice/float(365*24*60);
   float dayAngle           = 360 * minuteOfDay/(24*60.0) - yearAngle;

   QMatrix4x4 earthYearMatrix;
   earthYearMatrix.rotate(yearAngle,{0,1,0});

   QMatrix4x4 earthMatrix;

   earthMatrix.translate(QVector3D(EARTH_START_POSITION) * earthYearMatrix);
   earthMatrix.rotate(23.44, {0,0,1});       // Earth Axis: https://en.wikipedia.org/wiki/Axial_tilt

   earthMatrix.rotate(dayAngle-90, {0,1,0}); // 90Deg: Correction of the 3DModel reference...
   earthMatrix.scale(1.0/mModel->radius());
   mModelState->setModelView(earthMatrix);

   // Calculate ISS, orbit: 15x per day (~5760s)
   float issAngle = 360*((utc.time().hour()*3600*1000 + utc.time().minute()*60*1000 + utc.time().second()*1000 + utc.time().msec()) % 5760000)/float(5760000);
   QMatrix4x4 issMatrix;
   issMatrix.rotate(24+52,{0,0,1}); // 34: Earth-Axis + 52: ISS Inclination
   issMatrix.rotate(issAngle,{0,1,0});

   issMatrix.translate(QVector3D({0.6,0,0})); // Earth: 0.5 .. so 0.6 "near Earth.."

   mIssState->setPos(mModelState->pos() + issMatrix*(QVector3D({0,0,0})));
   ui->viewer->update();
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::setupSolarSystem()
{
    // Sun: center {0,0,0}
    {
        auto *mesh = ui->viewer->createMesh();
        Qtr3d::meshBySphere(*mesh,256,QImage(":/sun.jpg"));
        mesh->setFaceOrientation(Qtr3d::CounterClockWise);
        auto *state = ui->viewer->createState(mesh);
        state->setLightingType(Qtr3d::NoLighting);
        state->setRotation({90,0,0});
        state->setScale(3.0/mesh->radius());
    }

    // Globe
    // if (0)
    {
        mModel = ui->viewer->createModel();
        mModel->setFaceOrientation(Qtr3d::CounterClockWise);
        Qtr3d::modelByFile(*mModel,":/Earth_1_12756.glb");
        mModelState =  ui->viewer->createState(mModel);
        mModelState->setScale(1.0/mModel->radius());
    }

    // Vertical Axe
    // if (0)
    {
        auto *mesh = ui->viewer->createMesh();
        mesh->startMesh(Qtr3d::Line);
        mesh->addVertex({0.0,float(+2*mModel->radius()),0.0}, Qt::gray);
        mesh->addVertex({0.0,float(-2*mModel->radius()),0.0},Qt::gray);
        mesh->endMesh();
        mModel->addMesh(mesh,true);
    }

    // ISS
    // if (0)
    {
        auto *issModel = ui->viewer->createModel();
        issModel->setFaceOrientation(Qtr3d::CounterClockWise);
        Qtr3d::modelByFile(*issModel,":/ISS_stationary.glb");
        mIssState =  ui->viewer->createState(issModel);
        mIssState->setScale(0.01*(1.0/issModel->radius()));
        mIssState->setLightingType(Qtr3d::PhongLighting);
    }

    ui->viewer->primaryLightSource()->setPos({0,0,0});
    ui->viewer->primaryLightSource()->setAmbientStrength(0);

    mModel->material().ambient().strength = {0,0,0};
    mModel->material().diffuse().strength = {1.1,1.1,1.1};

    mModelState->setLightingType(Qtr3d::FlatLighting);

    setupStarDatabase();

    // Earth
    ui->viewer->camera()->lookAt(QVector3D(EARTH_START_POSITION) + QVector3D(5,0,0), EARTH_START_POSITION, {0,1,0});

    // Sun
    // ui->viewer->camera()->lookAt(QVector3D(100,0,0), {0,0,0}, {0,1,0});

    // Polaris
    // ui->viewer->camera()->lookAt(QVector3D(0,0,0), mPolar, {0,1,0});
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::setupStarDatabase()
{

    QFile inputFile(":/bsc5p_min.json");

    // QFile inputFile(":/BSC.json");
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

        if (map.contains("galacticLongitude")) { // bsc5p_min.json
            double longitude = map.value("galacticLongitude").toDouble();
            double latitude  = map.value("galacticLatitude").toDouble();
            double magnitude = map.value("visualMagnitude").toDouble();
            if (magnitude > maxMag)
                continue;

            double x = 10000 * cos((latitude/360.0)*2*3.1415) * cos((longitude/360.0)*2*3.1415);
            double y = 10000 * cos((latitude/360.0)*2*3.1415) * sin((longitude/360.0)*2*3.1415);
            double z = 10000 * sin((latitude/360.0)*2*3.1415);

            // minMag = qMin(minMag,magnitude);
            // maxMag = qMax(maxMag,magnitude);

            // double expMin = exp(minMag);
            // double expMax = exp(maxMag);
            // double expMag = exp(qMin(maxMag,magnitude));

            float color = (maxMag - magnitude)/float(maxMag);
            mesh->addVertex(QVector3D(x,y,z),QColor(color*255,color*255,color*255));
        }

        if (map.contains("MAG")) {
            double ra   = angleFromTime(map.value("RA").toString(), false);
            double dec  = angleFromTime(map.value("DEC").toString(), true);
            double magnitude = map.value("MAG").toDouble();
            if (magnitude > maxMag)
                continue;

            QVector3D pos(1,0,0);

            matrix = QMatrix4x4();
            matrix.rotate(ra,{0,1,0});
            pos = pos * matrix;

            matrix = QMatrix4x4();
            matrix.rotate(dec,QVector3D::crossProduct(pos,{0,1,0}));
            pos = pos * matrix;


            // Just Debugging... 424 is Polaris...
            if (map.value("harvard_ref_#").toInt() == 424) {

                ra   = angleFromTime(map.value("RA").toString(), false);
                dec  = angleFromTime(map.value("DEC").toString(), true);

                QVector3D pos(1,0,0);

                matrix = QMatrix4x4();
                matrix.rotate(ra,{0,1,0});
                pos = pos * matrix;

                for (int i=0; i<100; i++) {
                    mesh->addVertex(i*pos,Qt::green);
                }

                for (int i=0; i<100; i++) {
                    mesh->addVertex(i*QVector3D::crossProduct(pos,{0,1,0}),Qt::blue);
                }

                matrix = QMatrix4x4();
                matrix.rotate(dec,QVector3D::crossProduct(pos,{0,1,0}));
                pos = pos * matrix;


                mPolar = 1000*pos;
                mesh->addVertex(mPolar,Qt::red);
            } else {
                float color = qMin(qMax((maxMag-magnitude)/maxMag,0.0),1.0);
                mesh->addVertex(10000*pos,QColor(color*255,color*255,color*255));
            }
        }
    }

    mesh->endMesh(true);
    auto *state = ui->viewer->createState(mesh,Qtr3d::NoLighting);
}

//-------------------------------------------------------------------------------------------------
double ViewerForm::angleFromTime(const QString &time, bool firstIsGrad)
{
    // 00:14:02.30
    QStringList parts = time.split(":");
    if (parts.count() != 3)
        return 0.0;

    double hours   = parts[0].toDouble();
    double minutes = parts[1].toDouble()/60;
    double seconds = parts[2].toDouble()/3600;

    if (firstIsGrad)
        return hours + ((time.startsWith("-") ? (- minutes - seconds) : (+ minutes + seconds)) * 15);

    return (time.startsWith("-") ? (hours - minutes - seconds) : (hours + minutes + seconds)) * 15;
}



