
#include <QPushButton>
#include <QFileDialog>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dvertexmesh.h>
#include <libqtr3d/qtr3dcameracycler.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dmodelfactory.h>

#include "viewerform.h"
#include "ui_viewerform.h"

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

        // Sky
        Qtr3dVertexMesh *mesh;
        mesh = ui->viewer->createVertexMesh();
        Qtr3dModelFactory::meshByStarsky(*mesh,10000,1000,Qt::white);
        ui->viewer->createBufferState(mesh);

        mCameraMove = new Qtr3dCameraCycler(ui->viewer->camera(),30,0.3,{0,(float)(1000),(float)(2*1000)},{0,0,0});
    });

    connect(ui->btnLoad, &QPushButton::clicked, this, &ViewerForm::load);
}

ViewerForm::~ViewerForm()
{
    delete ui;
}

void ViewerForm::load()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home/jana", tr("3D Files (*.stl *.obj)"));

    if (fileName.isEmpty())
        return;

    if (mModel) {
        mModel->deleteLater();
        mModelState->deleteLater();
    }
    mModel = ui->viewer->createVertexMesh();
    mModel->setDefaultColor(Qt::white);


    Qtr3dModelFactory::meshByFile(*mModel,fileName);
    if (mModel->vertexCount() <= 0) {
        delete mModel;
        mModel = nullptr;
    }
    mModelState =  ui->viewer->createBufferState(mModel);
    mModelState->setFlat(false);

    if (mCameraMove)
        mCameraMove->deleteLater();
    mCameraMove = new Qtr3dCameraCycler(ui->viewer->camera(),30,0.3,{0,(float)(mModel->radius()),(float)(2*mModel->radius())},{0,0,0});
}
