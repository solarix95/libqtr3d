#include <QApplication>
#include <QDebug>
#include <QFont>
#include <QImage>
#include <QPainter>
#include <QVariantMap>
#include <functional>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dmesh.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dfactory.h>
#include <libqtr3d/qtr3dlightsource.h>
#include <libqtr3d/extras/qtr3dorbitcameracontroller.h>
#include "solarsystem.h"

static QImage showroomTexture()
{
    QImage img(96,96,QImage::Format_ARGB32);
    img.fill(QColor("#263238"));

    QPainter p(&img);
    p.fillRect(0,0,48,48,QColor("#ffca28"));
    p.fillRect(48,48,48,48,QColor("#ffca28"));
    p.fillRect(48,0,48,48,QColor("#26c6da"));
    p.fillRect(0,48,48,48,QColor("#ef5350"));
    p.setPen(QPen(Qt::white, 3));
    p.drawLine(0,0,96,96);
    p.drawLine(96,0,0,96);
    return img;
}

static QImage showroomHighmap()
{
    QImage img(24,24,QImage::Format_ARGB32);
    for (int y=0; y<img.height(); y++) {
        for (int x=0; x<img.width(); x++) {
            float dx = (x - img.width()/2.0f) / (img.width()/2.0f);
            float dy = (y - img.height()/2.0f) / (img.height()/2.0f);
            int value = qBound(0, int(255 * (1.0f - qMin(1.0f, dx*dx + dy*dy))), 255);
            img.setPixel(x, y, QColor(value,value,value).rgba());
        }
    }
    return img;
}

static QImage showroomColormap()
{
    QImage img(24,24,QImage::Format_ARGB32);
    for (int y=0; y<img.height(); y++) {
        for (int x=0; x<img.width(); x++) {
            int green = 90 + (120 * y) / img.height();
            int blue = 80 + (100 * x) / img.width();
            img.setPixel(x, y, QColor(70, green, blue).rgba());
        }
    }
    return img;
}

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w;
    w.setGeometry(10,10,900,650);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {

        w.setDefaultLighting(Qtr3d::FlatLighting);
        w.primaryLightSource()->setAmbientStrength(0.35);

        const QVector3D start(-5.4f, 0.0f, -3.0f);
        const float xStep = 3.6f;
        const float zStep = 3.0f;
        int itemIndex = 0;

        auto positionByIndex = [&](int index) {
            int col = index % 4;
            int row = index / 4;
            return QVector3D(start.x() + col*xStep, start.y(), start.z() + row*zStep);
        };

        auto addLabel = [&](const QString &name, const QVector3D &pos) {
            auto *label = w.createMesh();
            Qtr3d::meshByText(*label, name, QFont("Arial", 72), Qt::white, Qt::transparent);
            auto *labelState = w.createState(label, Qtr3d::NoLighting);
            labelState->setState(pos + QVector3D(0,-1.25f,0), {0,0,0}, {0.55f,0.55f,0.55f});
        };

        auto addItem = [&](const QString &name,
                           const std::function<bool(Qtr3dMesh&)> &build,
                           const QVector3D &rotation = QVector3D(0,0,0),
                           const QVector3D &scale = QVector3D(1,1,1),
                           Qtr3d::LightingType lighting = Qtr3d::FlatLighting) {
            auto *mesh = w.createMesh();
            if (!build(*mesh)) {
                qWarning() << "Factory showroom: failed to build" << name;
                return;
            }

            QVector3D pos = positionByIndex(itemIndex++);
            auto *state = w.createState(mesh, lighting);
            state->setState(pos, rotation, scale);
            addLabel(name, pos);
        };

        addItem("Box", [](Qtr3dMesh &mesh) {
            return Qtr3d::meshByBox(mesh, {1.4f,1.1f,1.0f}, QColor("#64b5f6"));
        }, {12,28,0});

        addItem("Plane", [](Qtr3dMesh &mesh) {
            return Qtr3d::meshByPlane(mesh, 1.8f, 1.8f, QColor("#81c784"));
        }, {0,0,0});

        addItem("Disc", [](Qtr3dMesh &mesh) {
            return Qtr3d::meshByCycle(mesh, 48, QColor("#ffb74d"));
        }, {90,0,0});

        addItem("Sphere", [](Qtr3dMesh &mesh) {
            return Qtr3d::meshBySphere(mesh, 32, QColor("#ba68c8"));
        });

        addItem("Texture", [](Qtr3dMesh &mesh) {
            return Qtr3d::meshByTexture(mesh, showroomTexture(), 1.7f, 1.7f);
        }, {70,0,0}, {1,1,1}, Qtr3d::NoLighting);

        addItem("Text", [](Qtr3dMesh &mesh) {
            return Qtr3d::meshByText(mesh, "Qtr3d", QFont("Arial", 96, QFont::Bold), QColor("#fff176"), Qt::transparent);
        }, {0,0,0}, {0.8f,0.8f,0.8f}, Qtr3d::NoLighting);

        addItem("Cylinder", [](Qtr3dMesh &mesh) {
            return Qtr3d::meshByCylinder(mesh, 40, 0.7f, 1.5f, true, true, QColor("#4db6ac"));
        }, {0,0,0});

        addItem("Tube", [](Qtr3dMesh &mesh) {
            return Qtr3d::meshByTube(mesh, 32, 0.75f, 1.5f, QColor("#90a4ae"));
        }, {0,0,0});

        addItem("Cone", [](Qtr3dMesh &mesh) {
            return Qtr3d::meshByCone(mesh, 40, 0.8f, 1.6f, true, QColor("#ef5350"));
        }, {0,0,0});

        addItem("Particle", [](Qtr3dMesh &mesh) {
            return Qtr3d::meshByParticleTriangle(mesh, QColor("#f06292"), 0.95f, true);
        }, {90,0,0}, {1,1,1}, Qtr3d::NoLighting);

        addItem("Chess", [](Qtr3dMesh &mesh) {
            return Qtr3d::meshByChessboard(mesh, 8, 1.8f, QColor("#eceff1"), QColor("#37474f"));
        }, {0,0,0}, {1,1,1}, Qtr3d::NoLighting);

        addItem("Highmap", [](Qtr3dMesh &mesh) {
            return Qtr3d::meshByHighmap(mesh, showroomHighmap(), showroomColormap(), {0.08f,1.0f,0.08f});
        }, {0,0,0}, {1,1,1});

        addItem("JSON", [](Qtr3dMesh &mesh) {
            QVariantMap shape;
            shape["shape"] = "cone";
            shape["sectors"] = 6;
            shape["radius"] = 0.85f;
            shape["height"] = 1.6f;
            shape["bottomClosed"] = true;
            shape["color"] = "#7e57c2";

            QVariantMap root;
            root["mesh"] = shape;
            return Qtr3d::meshByJson(mesh, root);
        }, {0,30,0});

        auto *axis = w.createMesh();
        Qtr3d::meshByXyzAxis(*axis, 2.0f);
        w.createState(axis, Qtr3d::NoLighting)->setPos({-6.8f,-1.35f,-4.2f});

        auto *sky = w.createMesh();
        Qtr3d::meshByStarsky(*sky, 1000, 1200, Qt::white);
        w.createState(sky, Qtr3d::NoLighting);

        new Qtr3dOrbitCameraController(w.camera(), 30, 0.25, {0,5.2f,12.5f}, {0,0,1.0f});
    });

    w.show();

    return app.exec();
}
