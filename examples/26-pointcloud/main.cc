#include <QApplication>
#include <QColor>
#include <QDebug>
#include <QKeyEvent>
#include <QPainter>
#include <QtMath>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dpointcloud.h>
#include <libqtr3d/qtr3dmesh.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dfactory.h>
#include <libqtr3d/qtr3dlightsource.h>
#include <libqtr3d/extras/qtr3dorbitcameracontroller.h>


class PointCloudExample : public Qtr3dWidget
{
public:
    explicit PointCloudExample(Options options)
        : Qtr3dWidget(options)
        , mRadius(55.0f)
        , mMode(1)
        , mOrbit(nullptr)
    {
        setFocusPolicy(Qt::StrongFocus);
    }

    ~PointCloudExample()
    {
        setOrbit(nullptr);
    }

    void setRadius(float radius)
    {
        mRadius = radius;
    }

    void setViewMode(int mode)
    {
        mMode = mode;
        const QVector3D center(0,0,0);
        const float r = mRadius;

        switch (mMode) {
        case 1:
            setOrbit(new Qtr3dOrbitCameraController(camera(), 30, 0.12f,
                                                    QVector3D(0, r*0.55f, r*2.2f),
                                                    center));
            break;
        case 2:
            setOrbit(nullptr);
            camera()->lookAt(QVector3D(0, r*2.4f, 0), center, QVector3D(0,0,-1));
            break;
        case 3:
            setOrbit(nullptr);
            camera()->lookAt(QVector3D(0, r*0.15f, r*2.15f), center, QVector3D(0,1,0));
            break;
        case 4:
            setOrbit(new Qtr3dOrbitCameraController(camera(), 30, 0.22f,
                                                    QVector3D(0, r*0.18f, r*0.68f),
                                                    center));
            break;
        case 5:
            setOrbit(new Qtr3dOrbitCameraController(camera(), 30, 0.06f,
                                                    QVector3D(0, r*0.95f, r*3.3f),
                                                    center));
            break;
        case 6:
            setOrbit(nullptr);
            camera()->lookAt(QVector3D(r*1.45f, r*0.9f, r*1.45f), center, QVector3D(0,1,0));
            break;
        default:
            break;
        }
        update();
    }

protected:
    void keyPressEvent(QKeyEvent *event) override
    {
        if (event->key() >= Qt::Key_1 && event->key() <= Qt::Key_6) {
            setViewMode(event->key() - Qt::Key_0);
            event->accept();
            return;
        }
        Qtr3dWidget::keyPressEvent(event);
    }

    void paint2D() override
    {
        Qtr3dWidget::paint2D();

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setPen(QColor(230,235,245));
        painter.setBrush(QColor(0,0,0,150));
        painter.drawRoundedRect(QRect(12,12,250,136), 6, 6);

        QFont font = painter.font();
        font.setPointSize(9);
        painter.setFont(font);

        const QStringList lines = {
            QStringLiteral("Pointcloud camera"),
            QStringLiteral("1  Orbit overview"),
            QStringLiteral("2  Top view"),
            QStringLiteral("3  Edge view"),
            QStringLiteral("4  Core orbit"),
            QStringLiteral("5  Wide halo orbit"),
            QStringLiteral("6  Diagonal view")
        };

        int y = 32;
        for (int i=0; i<lines.size(); ++i) {
            painter.setPen(i == 0 ? QColor(255,245,210) : QColor(225,230,240));
            painter.drawText(24, y, lines.at(i));
            y += 18;
        }
    }

private:
    void setOrbit(Qtr3dOrbitCameraController *controller)
    {
        if (mOrbit) {
            removeEventFilter(mOrbit);
            delete mOrbit;
        }
        mOrbit = controller;
        if (mOrbit)
            installEventFilter(mOrbit);
    }

    float mRadius;
    int mMode;
    Qtr3dOrbitCameraController *mOrbit;
};

static float pseudoRandom(int i, int salt)
{
    quint32 x = quint32(i * 747796405u + salt * 2891336453u + 277803737u);
    x = ((x >> ((x >> 28u) + 4u)) ^ x) * 277803737u;
    x = (x >> 22u) ^ x;
    return float(x & 0x00ffffffu) / float(0x00ffffffu);
}

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    PointCloudExample w(Qtr3dWidget::MSAA4);
    w.setGeometry(10,10,640,480);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {

        w.assets()->environment().setClearColor(QColor(5,7,13));
        w.setDefaultLighting(Qtr3d::NoLighting);

        auto *cloud = w.createPointCloud()->startCloud();

        const int armCount = 4;
        const int pointsPerArm = 18000;
        const float radius = 55.0f;
        const float height = 6.0f;
        w.setRadius(radius);

        for (int arm=0; arm<armCount; arm++) {
            const float armOffset = (2.0f * float(M_PI) * arm) / armCount;

            for (int i=0; i<pointsPerArm; i++) {
                const float t = pseudoRandom(i, arm);
                const float spread = pseudoRandom(i, arm + 17);
                const float twist = t * 5.5f;
                const float r = qSqrt(t) * radius;
                const float angle = armOffset + twist + (spread - 0.5f) * (0.8f + t * 1.5f);
                const float z = (pseudoRandom(i, arm + 31) - 0.5f) * height * (1.0f - t * 0.35f);

                QVector3D pos(qCos(angle) * r, z, qSin(angle) * r);
                QColor color;
                color.setHsvF(0.58f + 0.12f * (1.0f - t), 0.45f + 0.35f * t, 1.0f);
                cloud->addVertex(pos, color);
            }
        }

        for (int i=0; i<6000; i++) {
            const float t = pseudoRandom(i, 101);
            const float angle = pseudoRandom(i, 102) * 2.0f * float(M_PI);
            const float r = qPow(t, 1.8f) * radius * 1.25f;
            const float z = (pseudoRandom(i, 103) - 0.5f) * height * 1.8f;
            QColor color = i % 7 == 0 ? QColor(255,245,210) : QColor(120,150,255);
            cloud->addVertex(QVector3D(float(qCos(angle) * r), z, float(qSin(angle) * r)), color);
        }

        cloud->endCloud(true);
        w.createState(cloud, Qtr3d::NoLighting);

        auto *axis = w.createMesh();
        Qtr3d::meshByXyzAxis(*axis, radius * 0.35f);
        w.createState(axis, Qtr3d::NoLighting);

        w.camera()->setFov(50.0f, 0.5f, radius*20.0f);
        w.setViewMode(1);
        w.primaryLightSource()->setAmbientStrength(1.0f);
    });

    w.show();
    w.setFocus();

    return app.exec();
}

