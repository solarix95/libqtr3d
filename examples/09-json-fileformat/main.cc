#include <QApplication>
#include <QDebug>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dmesh.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dfactory.h>
#include <libqtr3d/extras/qtr3dcameracycler.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w;
    w.setGeometry(10,10,500,500);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {

        QList<Qtr3dGeometryBuffer*> buffers;

        {
            auto *mesh = w.createMesh();
            Qtr3d::meshByJson(*mesh,QString(":/mesh-lines.json"));
            buffers << mesh;
        }


        {
            auto *mesh = w.createMesh();
            Qtr3d::meshByJson(*mesh,QString(":/mesh-colored-cube.json"));
            buffers << mesh;
        }


        // Align Models in a cycle
        float      deltaAngle = 360 / buffers.count();
        float      showRoomRadius = 5;
        QVector3D  showRoomPos(0,0,showRoomRadius);
        QMatrix4x4 showRoomRotate;

        for (auto *buffer: buffers) {
            auto *state = w.createState(buffer);

            showRoomRotate.rotate(deltaAngle,{0,1,0});
            state->setScale(1/buffer->radius());
            state->setPos(showRoomRotate * showRoomPos);
        }

        // Sky
        {
            auto *mesh = w.createMesh();
            Qtr3d::meshByStarsky(*mesh,1000,1000,Qt::white);
            w.createState(mesh);
        }


        new Qtr3dCameraCycler(w.camera(),30,0.3,{0,showRoomRadius,showRoomRadius*3},{0,0,0});
    });

    w.show();

    return app.exec();
}
