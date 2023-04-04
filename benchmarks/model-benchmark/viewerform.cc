
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
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dlightsource.h>
#include <libqtr3d/qtr3dfactory.h>
#include <libqtr3d/physics/qtr3dfpsloop.h>
#include <libqtr3d/extras/qtr3dfreecameracontroller.h>
#include "viewerform.h"
#include "ui_viewerform.h"

//-------------------------------------------------------------------------------------------------
ViewerForm::ViewerForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ViewerForm)
{
    ui->setupUi(this);
    ui->viewer->setOptions(Qtr3dWidget::MSAA16);

    QObject::connect(ui->viewer, &Qtr3dWidget::initialized, [this]() {
        connect(ui->viewer,&MainView::statisticsChanged, this, [this]() {
            auto *context = ui->viewer->bufferContext();
            ui->lblTargetFps->setText(QString::number(context->loop().targetFps()));
            ui->lblCurrentFps->setText(QString::number(context->loop().currentFps()));
            ui->lblStateCount->setText(QString::number(ui->viewer->stateCount()));
            ui->lblLoad->setText(QString::number(ui->viewer->cpuLoad()));
            ui->lblModelCount->setText(QString("%1/%2/%3").arg(context->totalModelCount()).arg(context->totalMeshCount()).arg(context->totalVerticesCount()));
        }, Qt::QueuedConnection);
    });
}

//-------------------------------------------------------------------------------------------------
ViewerForm::~ViewerForm()
{
    delete ui;
}


