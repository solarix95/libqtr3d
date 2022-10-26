
INCLUDEPATH += libqtr3d $$LIBQTR3D_PATH
CONFIG += c++11
QT     += core gui widgets
INCLUDEPATH += $$LIBQTR3D_PATH

HEADERS += \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dtypes.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dwidget.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dshader.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dgeometrybuffer.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dgeometrybufferstate.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dcamera.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dcameramovement.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dcameracycler.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dtexturedquad.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dtexturedquadshader.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dvertexmesh.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dvertexmeshshader.h \
    $$LIBQTR3D_PATH/libqtr3d/debug/qtr3dfreecameracontroller.h \
    $$LIBQTR3D_PATH/libqtr3d/debug/qtr3dwidgetinputcontroller.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dmodelfactory.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dtexturefactory.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dmodel.h \
    $$LIBQTR3D_PATH/libqtr3d/physics/qtrphspace.h \
    $$LIBQTR3D_PATH/libqtr3d/physics/qtrphentity.h \
    $$LIBQTR3D_PATH/libqtr3d/physics/qtrphtimeloop.h \
    $$LIBQTR3D_PATH/libqtr3d/loader/qtr3dmodelloader.h \
    $$LIBQTR3D_PATH/libqtr3d/loader/qtr3dobjloader.h \
    $$LIBQTR3D_PATH/libqtr3d/utils/qtr3dutils.h \
    $$LIBQTR3D_PATH/libqtr3d/loader/qtr3dstlloader.h  \
    $$LIBQTR3D_PATH/libqtr3d/loader/qtr3dbinreader.h \
    $$LIBQTR3D_PATH/libqtr3d/loader/qtr3d3dsloader.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dtexturedmesh.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dtexturedmeshshader.h \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dgeometrybufferfactory.h

SOURCES += \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dwidget.cc \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dshader.cc \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dgeometrybuffer.cc \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dgeometrybufferstate.cc \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dcamera.cc \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dcameramovement.cc \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dcameracycler.cc \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dtexturedquad.cc \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dtexturedquadshader.cc \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dvertexmesh.cc \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dvertexmeshshader.cc \
    $$LIBQTR3D_PATH/libqtr3d/debug/qtr3dcameracontroller.cc \
    $$LIBQTR3D_PATH/libqtr3d/debug/qtr3dwidgetinputcontroller.cc \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dmodelfactory.cc \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dtexturefactory.cc \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dmodel.cc \
    $$LIBQTR3D_PATH/libqtr3d/physics/qtrphspace.cc \
    $$LIBQTR3D_PATH/libqtr3d/physics/qtrphentity.cc \
    $$LIBQTR3D_PATH/libqtr3d/physics/qtrphloop.cc \
    $$LIBQTR3D_PATH/libqtr3d/loader/qtr3dmodelloader.cc \
    $$LIBQTR3D_PATH/libqtr3d/loader/qtr3dobjloader.cc \
    $$LIBQTR3D_PATH/libqtr3d/utils/qtr3dutils.cc \
    $$LIBQTR3D_PATH/libqtr3d/loader/qtr3dstlloader.cc \
    $$LIBQTR3D_PATH/libqtr3d/loader/qtr3dbinreader.cc \
    $$LIBQTR3D_PATH/libqtr3d/loader/qtr3d3dsloader.cc \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dtexturedmesh.cc \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dtexturedmeshshader.cc \
    $$LIBQTR3D_PATH/libqtr3d/qtr3dgeometrybufferfactory.cc

RESOURCES += $$LIBQTR3D_PATH/libqtr3d/shaders.qrc

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

