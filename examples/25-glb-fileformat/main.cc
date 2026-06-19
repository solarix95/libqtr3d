#include <QApplication>
#include <QDebug>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dmesh.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dfactory.h>
#include <libqtr3d/qtr3dlightsource.h>
#include <libqtr3d/extras/qtr3dorbitcameracontroller.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w(Qtr3dWidget::MSAA4);

    w.setGeometry(10,10,500,500);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {

        w.setDefaultLighting(Qtr3d::FlatLighting);

        Qtr3dModel *model;
        Qtr3dGeometryState *state;

        // GLB-File
        model = w.createModel();
        model->setDefaultColor(Qt::white);

        // Duck model by https://github.com/KhronosGroup/glTF-Sample-Models
        if (!Qtr3d::modelByFile(*model,":/duck.glb")) {
            qWarning() << "Failed to load GLB model";
            return;
        }

        state = w.createState(model);
        state->setState({0,0,0},{0,0,0},{1,1,1});

        const QVector3D center = model->center();
        const float radius = model->radius() > 0 ? model->radius() : 1.0f;
        const float dist = radius*3.0f;

        // Sky
        auto *mesh = w.createMesh();
        Qtr3d::meshByStarsky(*mesh,dist*20.0f,1000,Qt::white);
        w.createState(mesh, Qtr3d::NoLighting);

        mesh = w.createMesh();
        Qtr3d::meshByStarsky(*mesh,dist*20.0f,100,Qt::blue);
        w.createState(mesh, Qtr3d::NoLighting);

        w.camera()->setFov(45.0f, radius*0.05f, dist*40.0f);
        new Qtr3dOrbitCameraController(w.camera(),30,0.2,
                                       center + QVector3D(0, radius*0.5f, dist),
                                       center);
        w.primaryLightSource()->setPos(center + QVector3D(0, radius, dist));
        w.primaryLightSource()->setAmbientStrength(0.2);
    });

    w.show();

    return app.exec();
}

