#include <QTimer>
#include <libqtr3d/qtr3dwidget.h>
#include <libqtr3d/qtr3dmodelfactory.h>
#include <libqtr3d/qtr3dvertexmesh.h>
#include <libqtr3d/qtr3dgeometrybufferstate.h>
#include "solarsystem.h"

//-------------------------------------------------------------------------------------------------
SolarSystem::SolarSystem(Qtr3dTextureFactory *textures, Qtr3dWidget *parent)
 : Qtr3dModel(textures)
{
    Qtr3dVertexMesh *mesh;
    Qtr3dGeometryBufferState *state;

    // Create the sun
    mesh = parent->createVertexMesh();
    Qtr3dModelFactory::meshBySphere(*mesh,13,Qt::yellow);
    state = parent->createBufferState(mesh,isFlat());
    state->setState({0,0,0},{0,0,0},{1,1,1});
    addState(state);

    // Create earth
    mesh = parent->createVertexMesh();
    Qtr3dModelFactory::meshBySphere(*mesh,13,Qt::blue);
    state = parent->createBufferState(mesh,isFlat());
    state->setState({3,0,0},{0,0,0},{0.5,0.5,0.5});
    addState(state);

    // Create Mars
    mesh = parent->createVertexMesh();
    Qtr3dModelFactory::meshBySphere(*mesh,13,Qt::red);
    state = parent->createBufferState(mesh,isFlat());
    state->setState({0,0,5},{0,0,0},{0.7,0.7,0.7});
    addState(state);

    QTimer *t = new QTimer(this);
    connect(t, &QTimer::timeout, this, &SolarSystem::updateSolarSystem);
    t->start(1000/30);
}

//-------------------------------------------------------------------------------------------------
void SolarSystem::updateSolarSystem()
{
    QMatrix4x4 rotation;
    rotation.rotate(1,{0,1,0});
    states()[1]->move(rotation * states()[1]->pos());
    rotation = QMatrix4x4();
    rotation.rotate(-3,{0,1,0});
    states()[2]->move(rotation * states()[2]->pos());
}
