
#include <QPushButton>
#include <QRadioButton>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QCursor>
#include <QFileDialog>
#include <QStandardPaths>
#include <QCoreApplication>
#include <QFileInfo>

#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dmodel.h>
#include <libqtr3d/qtr3dpointcloud.h>
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
    , mGeometry(nullptr)
    , mModel(nullptr)
    , mModelState(nullptr)
    , mCameraMove(nullptr)
{
    ui->setupUi(this);
    ui->btnLoad->setEnabled(false);
    ui->viewer->setOptions(Qtr3dWidget::MSAA4);

    QObject::connect(&ui->viewer->assets()->loop(), &Qtr3dFpsLoop::stepDone, ui->viewer,[&]() { ui->viewer->update();});
    QObject::connect(ui->viewer, &Qtr3dWidget::initialized, [&]() {

        ui->btnLoad->setEnabled(true);
        ui->viewer->camera()->lookAt({5,5,5},{0,0,0},{0,1,0});
        new Qtr3dFreeCameraController(ui->viewer);

        // Load from Command line Argument:
        for (auto arg: qApp->arguments()) {
            if (arg.startsWith("--load=")) {
                auto parts = arg.split("=");
                if (parts.count() == 2)
                    loadFile(parts[1]);
            }
        }

        // 3D Pivot Point
        auto *center = ui->viewer->createMesh();
        Qtr3d::meshByXyzAxis(*center, 5);
        ui->viewer->createState(center)->setLightingType(Qtr3d::NoLighting);

    });

    connect(ui->btnLoad, &QPushButton::clicked, this, &ViewerForm::load);
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
                                            tr("Open Point Cloud"), fileName, tr("PC Files (*.txt)"));

    if (fileName.isEmpty())
        return;

    loadFile(fileName);
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::loadFile(const QString &filename)
{
    if (mGeometry) {
        mGeometry->deleteLater();
        mGeometry   = nullptr;
        mModelState = nullptr;
        mModel      = nullptr;

        ui->lblName->setText("");
        ui->lblVertexCount->setText("n/a");
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    if (filename.toLower().endsWith(".txt"))
        loadEthSemantic3dTxt(filename);

     // TODO: PLY, LAZ
    // if (filename.toLower().endsWith(".ply"))
    //    loadEthSemantic3dTxt(filename);

    QApplication::restoreOverrideCursor();


    if (!mGeometry) // loading has failed...
        return;

    ui->lblName->setText(QFileInfo(filename).fileName());

    mModelState =  ui->viewer->createState(mGeometry);

    qDebug() << mGeometry->maxValues();
    qDebug() << mGeometry->minValues();

    qDebug() << mGeometry->radius() << (1/mGeometry->radius()) << mModelState->center();

    mModelState->setScale(1/mGeometry->radius());

    qDebug() << mModelState->center();

    // ui->viewer->camera()->lookAt(mModelState->center() + QVector3D({3,3,3}), mModelState->center(), {0,1,0});
    ui->viewer->camera()->lookAt(QVector3D({3,3,3}), {0,0,0}, {0,1,0});
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::loadEthSemantic3dTxt(const QString &filename)
{
    auto *mesh = ui->viewer->createPointCloud();

    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);

    int importLimit = ui->spbVertexLimit->value();
    int decimation  = ui->spbDecimation->value();

    mesh->startCloud();

    int count = 0;
    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList  fields = line.split(" ");
        if (fields.count() != 7)
            continue;

        if (++count % decimation)
            continue;

        // ETH Semantic3d TXT: Y/Z Switch to OpenGL-Standard:
        //                               X                    Z                    Y
        mesh->addVertex(QVector3D(fields[0].toFloat(), fields[2].toFloat(), fields[1].toFloat()),
        //                               R                  G                  B
                           QColor(fields[4].toInt(), fields[5].toInt(), fields[6].toInt()));

        if (importLimit > 0 && (mesh->verticesCount() >= importLimit))
            break;
    }

    mesh->endCloud(true);
    updateModelInfo(mesh->renderedVerticesCount());

    mGeometry = mesh;
}

//-------------------------------------------------------------------------------------------------
void ViewerForm::loadPly(const QString &filename)
{

}

//-------------------------------------------------------------------------------------------------
void ViewerForm::updateModelInfo(int vertexCount)
{
    ui->lblVertexCount->setText(QString("%1%2")
                                .arg(vertexCount)
                                .arg(vertexCount > 1000000 ? QString(" (%1M)").arg(vertexCount/1000000.0,0,'f',1) : QString()));

}
