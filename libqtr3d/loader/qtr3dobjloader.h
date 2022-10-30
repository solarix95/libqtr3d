#ifndef QTR3DOBJLOADER_H
#define QTR3DOBJLOADER_H

#include <QString>
#include <QStringList>
#include <QColor>
#include <QVector3D>
#include "qtr3dmodelloader.h"


class Qtr3dObjLoader : public Qtr3dModelLoader
{
public:
    static bool supportsFile(const QString &filename);
    static bool loadFile(Qtr3dModel &model, const QString &filename, Qtr3dGeometryBufferFactory &factory);

    Qtr3dObjLoader();
    virtual ~Qtr3dObjLoader();

    virtual bool loadModel(Qtr3dModel &model, const QString &filename, Qtr3dGeometryBufferFactory &factory);

protected:
    void processObject(const QStringList &args);
    void processVertex(const QStringList &args);
    void processNormal(const QStringList &args);
    void processFace(const QStringList &args);
    void processTextureCoords(const QStringList &args);
    void processSmoothshading(const QStringList &args);
    void processMaterialLib(const QString &sourcefile, const QStringList &args);
    void processMaterialTexture(const QString &matlibFilename, const QStringList &args);

private:
    void setupTexturedMesh(Qtr3dModel &model, Qtr3dGeometryBufferFactory &factory);
    void setupSimpleMesh(Qtr3dModel &model, Qtr3dGeometryBufferFactory &factory);
    static QString addPath(const QString &sourceFIle, const QString &targetFile);

    QList<QVector3D>  mVertices;
    QList<QColor>     mVerticesColors;

    QList<QVector3D>  mNormals;

    // Triangles
    QList<int>        mFaceVertexIndexes;
    QList<int>        mFaceTextureIndexes;
    QList<int>        mFaceNormalsIndexes;

    // Textured Model:
    QString           mTextureName;
    QList<QPointF>    mTextureCoords;

    Qtr3dVertexMesh *mMesh;
};

#endif // QTR3DOBJLOADER_H
