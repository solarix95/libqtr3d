#ifndef QTR3DE57LOADER_H
#define QTR3DE57LOADER_H

#include <QString>
#include <QFile>
#include <QList>
#include <QVariant>
#include <QVariantList>
#include <QStringList>
#include "qtr3dmodelloader.h"
#include "qtr3dbinreader.h"

class Qtr3dE57Loader : public Qtr3dModelLoader
{
public:
    static bool supportsFile(const QString &filename);
    static bool loadFile(Qtr3dModel &model, const QString &filename);

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
    Qtr3dE57Loader();
    virtual ~Qtr3dE57Loader();

    virtual bool loadModel(Qtr3dModel &model, const QString &filename);

private:
    bool parseHeader(Qtr3dBinReader &reader);
    bool decodeNextAsciiElement(QFile &f);
    bool decodeNextBinaryElement(Qtr3dBinReader &reader);
    void appendElementAttribut(const QString &elementName, const QString &attributName, const QVariantList &values);
    void appendElementAttributs(const QString &elementName, const QStringList &attributNames, const QVariantList &values);

    static AttributType typeByString(const QString &type);
    static bool         decodeAsciiVariant(const QString &inValue, QVariant &outValue, AttributType t);
    static bool         decodeBinaryVariant(Qtr3dBinReader &reader, QVariant &outValue, AttributType t);

    struct E57Header {
        char       fileSignature[8];
        quint32    majorVersion;
        quint32    minorVersion;
        quint64    filePhysicalLength;
        quint64    xmlPhysicalOffset;
        quint64    xmlLogicalLength;
        quint64    pageSize;
    };

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

    E57Header        mHeader;
    Format           mFormat;
    QList<Element>   mElements;
    Qtr3dMesh *mMesh;
};

#endif // QTR3DSTLLOADER_H
