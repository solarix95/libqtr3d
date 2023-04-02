
INCLUDEPATH += libqtr3d $$LIBQTR3D_PATH
CONFIG += c++11
QT     += core gui widgets
INCLUDEPATH += $$LIBQTR3D_PATH

HEADERS += \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dtypes.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dwidget.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dgeometry.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dgeometrystate.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dcamera.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dmesh.h \
    $$LIBQTR3D_PATH/libqtr3d/shaders/qtr3dshader.h \
    $$LIBQTR3D_PATH/libqtr3d/shaders/qtr3dplainshader.h \
    $$LIBQTR3D_PATH/libqtr3d/shaders/qtr3dvertexmeshshader.h \
    $$LIBQTR3D_PATH/libqtr3d/shaders/qtr3dtexturedmeshshader.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dfactory.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dtexturefactory.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dmodel.h \
    $$LIBQTR3D_PATH/libqtr3d/physics/qtr3dabstractspace.h \
    $$LIBQTR3D_PATH/libqtr3d/physics/qtr3dabstractentity.h \
    $$LIBQTR3D_PATH/libqtr3d/physics/qtr3dstandardentity.h \
    $$LIBQTR3D_PATH/libqtr3d/physics/qtr3dfpsloop.h \
    $$LIBQTR3D_PATH/libqtr3d/physics/qtr3dforcefield.h \
    $$LIBQTR3D_PATH/libqtr3d/loader/qtr3dmodelloader.h \
    $$LIBQTR3D_PATH/libqtr3d/loader/qtr3dobjloader.h \
    $$LIBQTR3D_PATH/libqtr3d/loader/qtr3dstlloader.h  \
    $$LIBQTR3D_PATH/libqtr3d/loader/qtr3dplyloader.h  \
    $$LIBQTR3D_PATH/libqtr3d/loader/qtr3dbinreader.h \
    $$LIBQTR3D_PATH/libqtr3d/loader/qtr3d3dsloader.h \
    $$LIBQTR3D_PATH/libqtr3d/loader/qtr3dglbloader.h \
    $$LIBQTR3D_PATH/libqtr3d/utils/qtr3dutils.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dlightsource.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dbillboard.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dcontext.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dsprite.h \
    $$LIBQTR3D_PATH/libqtr3d/extras/qtr3dfreecameracontroller.h \
    $$LIBQTR3D_PATH/libqtr3d/extras/qtr3degocameracontroller.h \
    $$LIBQTR3D_PATH/libqtr3d/extras/qtr3dwidgetinputcontroller.h \
    $$LIBQTR3D_PATH/libqtr3d/extras/qtr3dfollowcamera.h \
    $$LIBQTR3D_PATH/libqtr3d/extras/qtr3dcameramovement.h \
    $$LIBQTR3D_PATH/libqtr3d/extras/qtr3dcameracycler.h

SOURCES += \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dwidget.cc \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dgeometry.cc \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dgeometrystate.cc \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dcamera.cc \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dmesh.cc \
    $$LIBQTR3D_PATH/libqtr3d/shaders/qtr3dshader.cc \
    $$LIBQTR3D_PATH/libqtr3d/shaders/qtr3dvertexmeshshader.cc \
    $$LIBQTR3D_PATH/libqtr3d/shaders/qtr3dplainshader.cc \
    $$LIBQTR3D_PATH/libqtr3d/shaders/qtr3dtexturedmeshshader.cc \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dfactory.cc \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dtexturefactory.cc \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dmodel.cc \
    $$LIBQTR3D_PATH/libqtr3d/physics/qtr3dabstractspace.cc \
    $$LIBQTR3D_PATH/libqtr3d/physics/qtr3dabstractentity.cc \
    $$LIBQTR3D_PATH/libqtr3d/physics/qtr3dfpsloop.cc \
    $$LIBQTR3D_PATH/libqtr3d/physics/qtr3dstandardentity.cc  \
    $$LIBQTR3D_PATH/libqtr3d/physics/qtr3dforcefield.cc \
    $$LIBQTR3D_PATH/libqtr3d/loader/qtr3dmodelloader.cc \
    $$LIBQTR3D_PATH/libqtr3d/loader/qtr3dobjloader.cc \
    $$LIBQTR3D_PATH/libqtr3d/loader/qtr3dstlloader.cc \
    $$LIBQTR3D_PATH/libqtr3d/loader/qtr3dplyloader.cc \
    $$LIBQTR3D_PATH/libqtr3d/loader/qtr3dbinreader.cc \
    $$LIBQTR3D_PATH/libqtr3d/loader/qtr3d3dsloader.cc \
    $$LIBQTR3D_PATH/libqtr3d/loader/qtr3dglbloader.cc \
    $$LIBQTR3D_PATH/libqtr3d/utils/qtr3dutils.cc \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dlightsource.cc \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dbillboard.cc \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dcontext.cc \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dsprite.cc \
    $$LIBQTR3D_PATH/libqtr3d/extras/qtr3dcameracontroller.cc \
    $$LIBQTR3D_PATH/libqtr3d/extras/qtr3degocameracontroller.cc \
    $$LIBQTR3D_PATH/libqtr3d/extras/qtr3dwidgetinputcontroller.cc \
    $$LIBQTR3D_PATH/libqtr3d/extras/qtr3dfollowcamera.cc \
    $$LIBQTR3D_PATH/libqtr3d/extras/qtr3dcameracycler.cc \
    $$LIBQTR3D_PATH/libqtr3d/extras/qtr3dcameramovement.cc


RESOURCES += $$LIBQTR3D_PATH/libqtr3d/shaders.qrc
RESOURCES += $$LIBQTR3D_PATH/libqtr3d/misc.qrc

with_libassimp {
HEADERS += \
    $$LIBQTR3D_PATH/libqtr3d/loader/qtr3dassimploader.h

SOURCES += \
    $$LIBQTR3D_PATH/libqtr3d/loader/qtr3dassimploader.cc

LIBS += -lassimp

DEFINES += WITH_LIBASSIMP

}
#AbstractShape
# - Line
# - Triangle
# - Quad
# - TexturedQuad
# - Sphere
# Model
# Individum/Character/Player/
# Texture
# Camera

# RESOURCES += 
# LIBS +=
QT += core gui widgets

