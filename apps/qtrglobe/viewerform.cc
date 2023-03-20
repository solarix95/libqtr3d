
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
    ui->viewer->setOptions(Qtr3dWidget::MSAA4);

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

    // Earth
    //ui->viewer->camera()->lookAt(QVector3D(100-2*mModel->radius(),0,0), {100,0,0}, {0,1,0});

    // Sun
    // ui->viewer->camera()->lookAt(QVector3D(100,0,0), {0,0,0}, {0,1,0});

    ui->viewer->primaryLightSource()->setPos({0,0,0});
    ui->viewer->primaryLightSource()->setAmbientStrength(0);

    mModel->material().ambient().strength = {0,0,0};
    mModel->material().diffuse().strength = {1.1,1.1,1.1};

    mModelState->setLightingType(Qtr3d::FlatLighting);

    setupStarDatabase();

    // Earth
    //ui->viewer->camera()->lookAt(QVector3D(100-2*mModel->radius(),0,0), {100,0,0}, {0,1,0});

    // Sun
    // ui->viewer->camera()->lookAt(QVector3D(100,0,0), {0,0,0}, {0,1,0});

    // Polaris
    ui->viewer->camera()->lookAt(QVector3D(0,0,0), mPolar, {0,1,0});
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::setupStarDatabase()
{

    // QFile inputFile(":/bsc5p_min.json");

    QFile inputFile(":/BSC.json");
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

            double expMin = exp(minMag);
            double expMax = exp(maxMag);
            double expMag = exp(qMin(maxMag,magnitude));

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
                mesh->addVertex(1000*pos,QColor(color*255,color*255,color*255));
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



