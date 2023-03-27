
#include "hawksim.h"
#include "hawk.h"

#include <libqtr3d/qtr3dmesh.h>
#include <libqtr3d/qtr3dmodel.h>
#include <libqtr3d/qtr3dtypes.h>
#include <libqtr3d/qtr3dcamera.h>
#include <libqtr3d/qtr3dcontext.h>
#include <libqtr3d/qtr3dfactory.h>
#include <libqtr3d/extras/qtr3dcameracycler.h>
#include <libqtr3d/physics/qtrphspace.h>

HawkSim::HawkSim()
 : mCamera(nullptr)
{
}

void HawkSim::init(Qtr3dContext *context, Qtr3dCamera *camera)
{
    mCamera = camera;

    context->environment().setClearColor(QColor("#87ceeb"));
    context->environment().setFogDistance(1500);

    auto *mesh = context->createMesh();

    // by https://heightmap.skydark.pl/ LOVE IT!!
    QString hightmap  = ":/heightmap.png";
    QString colormap  = ":/texture.jpg";

    Qtr3d::meshByHighmap(*mesh,hightmap, QImage(colormap), {1,150,1});

    context->createState(mesh)->setLightingType(Qtr3d::NoLighting);

    auto *hawk = context->createModel();
    Qtr3d::modelByFile(*hawk,":GlobalHawk/GlobalHawkOBJ.obj");
    hawk->setFaceOrientation(Qtr3d::CounterClockWise);

    auto *hawkState = context->createState(hawk,Qtr3d::PhongLighting);
    hawkState->setPos({0,100,0});

    hawkState->setLightingType(Qtr3d::PhongLighting);

    context->space().append(new Hawk(*hawkState));

    // hawkState->setScale(0.5);

    new Qtr3dCameraCycler(mCamera,50,0.05,{0,200,500},{0,0,0});
}
