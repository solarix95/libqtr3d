#include <QApplication>
#include <QTimer>
#include <QDebug>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dvertexmesh.h>
#include <libqtr3d/qtr3dcameracycler.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/debug/qtr3dfreecameracontroller.h>

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w;
    w.setGeometry(50,50,500,500);

    int fanState = 0;
    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {

        // Lets create just one "blade":
        auto *mesh = w.createVertexMesh()->startMesh(Qtr3dVertexMesh::Triangle, Qtr3dGeometryBuffer::CounterClockWise);
        mesh->addVertex({0,0,0},Qt::red);
        mesh->addVertex({-1,10,-1},Qt::green);
        mesh->addVertex({1,10,+1},Qt::blue);
        mesh->endMesh();

        // .. done. now we need 16 "blades" to create one "fan"
        const int bladeCount = 16;
        for (int i=0; i<bladeCount; i++)
            mesh->addModelTransition({0,0,0},{0,0,i*(360.0f/bladeCount)});

        // .. and now we create one "fan"
        auto *turboFan = w.createBufferState(mesh);

        // ... and rotate the complete fan:
        QTimer *t = new QTimer(&w);
        QObject::connect(t, &QTimer::timeout, &w, [&, turboFan]() {
            turboFan->setRotation({0.0f,0.0f,fanState*2.0f});
            fanState++;
            w.update(); // don't forget to update the view..
        });
        t->start(25);

        // that's all... set camera an go..
        w.camera()->lookAt({0,0,-50},{0,0,0}, {0,1,0});
    });

    w.show();

    return app.exec();
}
