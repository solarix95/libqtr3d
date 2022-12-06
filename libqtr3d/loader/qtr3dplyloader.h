#ifndef QTR3DPLYLOADER_H
#define QTR3DPLYLOADER_H

#include <QString>
#include <QFile>
#include <QList>
#include <QVariant>
#include <QVariantList>
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


    enum AttributType {
        InvalidType,
        Char,
        UChar,
        Short,
        UShort,
        Int,
        UInt,
        Float,
        Double
    };
    Qtr3dPlyLoader();
    virtual ~Qtr3dPlyLoader();

    virtual bool loadModel(Qtr3dModel &model, const QString &filename, Qtr3dGeometryBufferFactory &factory);

private:
    void parseHeader(QFile &f);
    bool decodeNextAsciiElement(QFile &f);
    void appendElementAttribut(const QString &elementName, const QString &attributName, const QVariantList &values);
    void appendElementAttributs(const QString &elementName, const QStringList &attributNames, const QVariantList &values);

    static AttributType typeByString(const QString &type);
    static bool         decodeAsciiVariant(const QString &inValue, QVariant &outValue, AttributType t);

    // Element-Block Metadata for Decoding
    struct Element {
        // element vertex 8
        QString        name; // "vertex" or "face"
        int            elementCount;

        // property float x
        QList<QString>      attributNames;
        QList<AttributType> attributTypes;

        // property list uchar int vertex_index
        AttributType   listType; // uchar

        Element() : elementCount(0), listType(InvalidType) {}
        Element(QString n, int count): name(n), elementCount(count), listType(InvalidType) {}
    };

    Format           mFormat;
    QList<Element>   mElements;
    Qtr3dVertexMesh *mMesh;
};

#endif // QTR3DSTLLOADER_H
