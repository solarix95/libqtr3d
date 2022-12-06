#ifndef QTR3DPLYLOADER_H
#define QTR3DPLYLOADER_H

#include <QString>
#include <QFile>
#include <QList>
#include <QStringList>
#include "qtr3dmodelloader.h"

class Qtr3dPlyLoader : public Qtr3dModelLoader
{
public:
    static bool supportsFile(const QString &filename);
    static bool loadFile(Qtr3dModel &model, const QString &filename, Qtr3dGeometryBufferFactory &factory);

    enum Format {
        InvalidFormat,
        TextFormat,
        BinaryLittleEndianFormat,
        BinaryBigEndianFormat
    };
    Qtr3dPlyLoader();
    virtual ~Qtr3dPlyLoader();

    virtual bool loadModel(Qtr3dModel &model, const QString &filename, Qtr3dGeometryBufferFactory &factory);

protected:

private:
    void parseHeader(QFile &f);
    bool fromASCII(Qtr3dVertexMesh &mesh, QFile &f);
    bool fromBinary(Qtr3dVertexMesh &mesh, QFile &f);
    static QVector3D vectorFromStringList(const QStringList &strings);

    struct Element {
        // element vertex 8
        QString        name; // "vertex" or "face"
        int            elementCount;

        // property float x
        QList<QString> attributNames;
        QList<int>     attribuTypes;

        // property list uchar int vertex_index
        int  listType; // uchar

        Element() : elementCount(0), listType(QVariant::Invalid) {}
    };

    Format           mFormat;
    QList<Element>   mElements;
    Qtr3dVertexMesh *mMesh;
};

#endif // QTR3DSTLLOADER_H
