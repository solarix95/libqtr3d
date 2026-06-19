#include <QApplication>
#include <QColor>
#include <QDebug>
#include <QImage>
#include <QPainter>
#include <QRadialGradient>
#include <QTimer>

#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dfactory.h>
#include <libqtr3d/qtr3dgeometrystate.h>
#include <libqtr3d/qtr3dmesh.h>
#include <libqtr3d/qtr3dtexturefactory.h>
#include <libqtr3d/extras/qtr3dsprite.h>
#include <libqtr3d/extras/qtr3dorbitcameracontroller.h>

static QImage spriteFrame(int frame, int frameCount)
{
    const int size = 192;
    const float t = frame / float(frameCount);
    const QPointF center(size/2.0, size/2.0);
    const float coreRadius = 12.0f + 24.0f*t;
    const float glowRadius = 34.0f + 64.0f*t;

    QImage img(size, size, QImage::Format_ARGB32_Premultiplied);
    img.fill(Qt::transparent);

    QPainter p(&img);
    p.setRenderHint(QPainter::Antialiasing, true);

    QRadialGradient glow(center, glowRadius);
    glow.setColorAt(0.0, QColor(255, 245, 180, 235));
    glow.setColorAt(0.25, QColor(255, 160, 35, 210));
    glow.setColorAt(0.62, QColor(200, 35, 20, 110));
    glow.setColorAt(1.0, QColor(40, 20, 80, 0));
    p.setBrush(glow);
    p.setPen(Qt::NoPen);
    p.drawEllipse(center, glowRadius, glowRadius);

    QColor ringColor(255, 230, 140, int(220 * (1.0f - t)));
    QPen ringPen(ringColor, 4.0 + 8.0*t);
    p.setBrush(Qt::NoBrush);
    p.setPen(ringPen);
    p.drawEllipse(center, coreRadius + 34.0f*t, coreRadius + 34.0f*t);

    QRadialGradient core(center, coreRadius);
    core.setColorAt(0.0, QColor(255, 255, 255, 255));
    core.setColorAt(0.35, QColor(255, 235, 90, 230));
    core.setColorAt(1.0, QColor(255, 80, 20, int(150 * (1.0f - t))));
    p.setPen(Qt::NoPen);
    p.setBrush(core);
    p.drawEllipse(center, coreRadius, coreRadius);

    return img;
}

int main(int argc, char *argv[])
{
    QApplication app(argc,argv);

    Qtr3dWidget w(Qtr3dWidget::MSAA4);
    w.setGeometry(50,50,640,480);

    QObject::connect(&w, &Qtr3dWidget::initialized, [&]() {

        w.assets()->environment().setClearColor(QColor(8,9,16));
        w.setDefaultLighting(Qtr3d::NoLighting);

        QList<Qtr3dTexture*> frames;
        const int frameCount = 24;
        for (int i=0; i<frameCount; i++)
            frames << w.textures()->texture(spriteFrame(i, frameCount), QString("sprite-frame-%1").arg(i));

        auto *spriteMesh = w.createMesh();
        spriteMesh->startMesh(Qtr3d::Triangle);
        spriteMesh->setRenderOptions(Qtr3d::BlendingOptions);
        spriteMesh->setTexture(frames.first());
        spriteMesh->addQuad({-2, 2, 0}, { 2, 2, 0}, { 2,-2, 0}, {-2,-2, 0});
        spriteMesh->endMesh();

        auto *spriteState = w.createState(spriteMesh, Qtr3d::NoLighting);
        auto *sprite = new Qtr3dSprite(spriteState, frames, w.camera());
        sprite->setTexture(0);

        auto *sky = w.createMesh();
        Qtr3d::meshByStarsky(*sky,1000,1200,Qt::white);
        w.createState(sky, Qtr3d::NoLighting);

        auto *timer = new QTimer(&w);
        QObject::connect(timer, &QTimer::timeout, [sprite, spriteState, frameCount]() {
            static int frame = 0;
            sprite->setTexture(frame % frameCount);
            const float scale = 1.0f + 0.25f * ((frame % frameCount) / float(frameCount));
            spriteState->setScale(scale);
            frame++;
        });
        timer->start(45);

        new Qtr3dOrbitCameraController(w.camera(),30,0.18,{0,0,12},{0,0,0});
    });

    w.show();

    return app.exec();
}
