#include <QApplication>
#include <QEvent>
#include <QKeyEvent>
#include <QLabel>
#include <QMatrix4x4>
#include <QTimer>
#include <QtMath>

#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dcameracontroller.h>
#include <libqtr3d/qtr3dfactory.h>
#include <libqtr3d/qtr3dmesh.h>
#include <libqtr3d/qtr3dgeometrystate.h>
#include <libqtr3d/qtr3dlightsource.h>
#include <libqtr3d/extras/qtr3dthirdpersoncameracontroller.h>


class CameraModeKeys : public QObject
{
public:
    CameraModeKeys(Qtr3dWidget *viewer, Qtr3dGeometryState *target, QObject *parent = nullptr)
        : QObject(parent)
        , mViewer(viewer)
        , mTarget(target)
    {
        setMode(2);
        viewer->installEventFilter(this);
    }

    void setTargetDirection(const QVector3D &direction)
    {
        if (auto *thirdPerson = qobject_cast<Qtr3dThirdPersonCameraController*>(mViewer->cameraController()))
            thirdPerson->setTargetDirection(direction);
    }

protected:
    bool eventFilter(QObject *watched, QEvent *event) override
    {
        Q_UNUSED(watched);
        if (event->type() != QEvent::KeyPress)
            return false;

        auto *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() >= Qt::Key_1 && keyEvent->key() <= Qt::Key_6) {
            setMode(keyEvent->key() - Qt::Key_0);
            return true;
        }
        return false;
    }

private:
    void setMode(int mode)
    {
        switch (mode) {
        case 1:
            mViewer->camera()->lookAt({0,8,18},{0,0,0},{0,1,0});
            mViewer->setCameraController(Qtr3d::freeFlyCamera());
            break;
        case 2:
            mViewer->setCameraController(Qtr3d::orbitCamera(mTarget, 22));
            break;
        case 3:
            mViewer->setCameraController(Qtr3d::firstPersonCamera({0,4,18}));
            break;
        case 4:
            mViewer->setCameraController(Qtr3d::thirdPersonCamera(mTarget, 12, 4));
            break;
        case 5:
            mViewer->setCameraController(Qtr3d::chaseCamera(mTarget, 16, 5));
            break;
        case 6:
            mViewer->setCameraController(Qtr3d::topDownCamera(mTarget, 35));
            break;
        default:
            break;
        }
    }

    Qtr3dWidget *mViewer;
    Qtr3dGeometryState *mTarget;
};

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w(Qtr3dWidget::MSAA4);
    w.setGeometry(50,50,800,500);
    w.setFocusPolicy(Qt::StrongFocus);

    auto *help = new QLabel(&w);
    help->setText("Camera controllers\n"
                  "1  Free fly\n"
                  "2  Orbit target\n"
                  "3  First person\n"
                  "4  Third person\n"
                  "5  Chase camera\n"
                  "6  Top down");
    help->setStyleSheet("QLabel { background: rgba(0,0,0,155); color: rgb(225,230,240); "
                        "padding: 10px 14px; border-radius: 6px; font-size: 9pt; }");
    help->move(12,12);
    help->adjustSize();
    help->show();

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {
        w.setDefaultLighting(Qtr3d::FlatLighting);
        w.primaryLightSource()->setPos({0,20,20});
        w.primaryLightSource()->setAmbientStrength(0.35f);
        w.camera()->setFov(50, 0.1f, 1000.0f);

        auto *axis = w.createMesh();
        Qtr3d::meshByXyzAxis(*axis, 6);
        w.createState(axis, Qtr3d::NoLighting);

        auto *ground = w.createMesh();
        Qtr3d::meshByChessboard(*ground, 24, 24, QColor(45,45,45), QColor(75,75,75));
        auto *groundState = w.createState(ground, Qtr3d::NoLighting);
        groundState->setPos({0,-1,0});

        auto *targetMesh = w.createMesh();
        Qtr3d::meshBySphere(*targetMesh, 24, QColor(230,80,60));
        auto *targetState = w.createState(targetMesh, Qtr3d::FlatLighting);
        targetState->setScale(1.2f);

        auto *keys = new CameraModeKeys(&w, targetState, &w);

        auto *timer = new QTimer(&w);
        QObject::connect(timer, &QTimer::timeout, [targetState, keys, &w]() {
            static float angle = 0;
            angle += 1.0f;
            QMatrix4x4 turn;
            turn.rotate(angle, {0,1,0});
            QVector3D pos = turn * QVector3D(8,0,0);
            QVector3D dir = QVector3D(-pos.z(), 0, pos.x()).normalized();
            targetState->setState(pos, {0,angle,0}, {1.2f,1.2f,1.2f});
            keys->setTargetDirection(dir);
            w.update();
        });
        timer->start(33);
    });

    w.show();
    w.setFocus();
    return app.exec();
}
