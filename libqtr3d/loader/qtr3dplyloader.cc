#include <QFile>
#include "qtr3dplyloader.h"
#include <libqtr3d/loader/qtr3dbinreader.h>

//-------------------------------------------------------------------------------------------------
bool Qtr3dPlyLoader::supportsFile(const QString &filename)
{
    return (fileHeader(filename).startsWith("ply\n"));
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dPlyLoader::loadFile(Qtr3dModel &model, const QString &filename, Qtr3dGeometryBufferFactory &factory)
{
    Qtr3dPlyLoader loader;
    return loader.loadModel(model,filename,factory);
}

//-------------------------------------------------------------------------------------------------
Qtr3dPlyLoader::Qtr3dPlyLoader()
    : mFormat(InvalidFormat)
    , mMesh(nullptr)
{
}

//-------------------------------------------------------------------------------------------------
Qtr3dPlyLoader::~Qtr3dPlyLoader() = default;

//-------------------------------------------------------------------------------------------------
bool Qtr3dPlyLoader::loadModel(Qtr3dModel &model, const QString &filename, Qtr3dGeometryBufferFactory &factory)
{
    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly))
        return false;

    auto magicLine = f.readLine();
    if (!magicLine.startsWith("ply"))
        return false;

    parseHeader(f);

    if (mFormat == InvalidFormat)
        return false;

    mMesh = factory.createVertexMesh();
    mMesh->setDefaultColor(model.defaultColor());

    mMesh->startMesh(Qtr3dVertexMesh::Triangle);
    if (mFormat == TextFormat) {
        while (mElements.count() > 0)
            if (!decodeNextAsciiElement(f)) {
                delete mMesh;
                return false;
            }
    }
    mMesh->endMesh();

    model.addGeometry(mMesh);
    return true;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dPlyLoader::parseHeader(QFile &f)
{
    QByteArray nextLine;
    do {
        nextLine   = f.readLine().toLower().trimmed();
        if (nextLine.startsWith("comment"))
            continue;

        auto parts = nextLine.split(' ');
        if (parts.count() >= 3) {
            if (parts[0] == "format") { // "format binary_big_endian 1.0"
                if (parts[1] == "binary_big_endian")
                    mFormat = BinaryBigEndianFormat;
                else if (parts[1] == "binary_little_endian")
                    mFormat = BinaryLittleEndianFormat;
                else if (parts[1] == "ascii")
                    mFormat = TextFormat;
            }
            if (parts[0] == "element") { // "element vertex 8"
                mElements << Element(parts[1], parts[2].toInt());
            }
            /*
                property float nz
            oder
                property list uchar int vertex_indices
            */
            if (parts[0] == "property") {
                if (mElements.isEmpty()) {
                    mFormat = InvalidFormat; // Abort parsing
                    return;
                }

                if (parts[1] == "list") {
                    mElements.last().listType = typeByString(parts[2]);
                    if (parts.count() != 5 || !mElements.last().attributTypes.isEmpty() || mElements.last().listType == InvalidType) {
                        mFormat = InvalidFormat; // Abort parsing
                        return;
                    }
                    mElements.last().attributTypes << typeByString(parts[3]);
                    QString attrName = parts[4];
                    if (attrName == "vertex_indices") // handle "indices" as "index"
                        attrName = "vertex_index";
                    mElements.last().attributNames << attrName;
                } else if (parts.count() == 3) {
                    mElements.last().attributTypes << typeByString(parts[1]); // "int"
                    mElements.last().attributNames << parts[2];               // "x" or "y".. or "nx"
                }
            }
        }
    } while (!nextLine.isEmpty() && (nextLine != "end_header"));
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dPlyLoader::decodeNextAsciiElement(QFile &f)
{
    Q_ASSERT(mElements.count() > 0);
    Element e = mElements.takeFirst();
    if (e.attributTypes.isEmpty())
        return false;

    QList<QByteArray> parts;
    QVariant          value;
    QVariantList      values;
    for (int i=0; i<e.elementCount; i++) {
        parts = f.readLine().trimmed().simplified().split(' ');
        if (parts.isEmpty())
            return false;

        // Decode List Attribute
        if (e.listType != InvalidType) {
            // "property list uchar int vertex_indices" -> "3 1 2 3"
            if (!decodeAsciiVariant(parts[0],value,e.listType))
                return false;
            parts.takeFirst();
            if (value.toInt() != parts.count())
                return false;
            values.clear();
            while (parts.count()) {
                if (!decodeAsciiVariant(parts.takeFirst(),value,e.attributTypes.first()))
                    return false;
                values << value;
            }
            appendElementAttribut(e.name, e.attributNames.first(), values);
            continue;
        }

        // Decode Recular/Scalar Attribute
        if (parts.count() != e.attributNames.count())
            return false;
        values.clear();
        for (int ai=0; ai<parts.count(); ai++) {
            if (!decodeAsciiVariant(parts[ai],value,e.attributTypes[ai]))
                return false;
            values << value;
        }
        appendElementAttributs(e.name, e.attributNames, values);

    }

    return true;
}

//-------------------------------------------------------------------------------------------------
void Qtr3dPlyLoader::appendElementAttribut(const QString &elementName, const QString &attributName, const QVariantList &values)
{
    // qDebug() << elementName << attributName << values;
    if (elementName == "face" && attributName == "vertex_index") {
        if (values.count() == 3 || values.count() == 4) { // Triangle
            mMesh->addIndex(values[0].toInt());
            mMesh->addIndex(values[1].toInt());
            mMesh->addIndex(values[2].toInt());
        }
        if (values.count() == 4) { // 2nd Triangle of Quad
            mMesh->addIndex(values[0].toInt());
            mMesh->addIndex(values[2].toInt());
            mMesh->addIndex(values[3].toInt());
        }
    }
}

//-------------------------------------------------------------------------------------------------
void Qtr3dPlyLoader::appendElementAttributs(const QString &elementName, const QStringList &attributNames, const QVariantList &values)
{
    // qDebug() << elementName << attributNames << values;
    if (elementName != "vertex")
        return;

    Qtr3dColoredVertex v;
    v.c   = mMesh->defaultColorF();
    v.p.x = values.value(attributNames.indexOf("x"),0.0).toFloat();
    v.p.y = values.value(attributNames.indexOf("y"),0.0).toFloat();
    v.p.z = values.value(attributNames.indexOf("z"),0.0).toFloat();
    v.n.x = values.value(attributNames.indexOf("nx"),0.0).toFloat();
    v.n.x = values.value(attributNames.indexOf("ny"),0.0).toFloat();
    v.n.x = values.value(attributNames.indexOf("nz"),0.0).toFloat();

    v.c.x = values.value(attributNames.indexOf("red"),  v.c.x).toFloat();
    v.c.y = values.value(attributNames.indexOf("green"),v.c.y).toFloat();
    v.c.z = values.value(attributNames.indexOf("blue"), v.c.z).toFloat();
    mMesh->addVertex(v);
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dPlyLoader::fromASCII(Qtr3dVertexMesh &mesh, QFile &f)
{
    QTextStream in(&f);
    mMesh = &mesh;

    QString     line;
    QString     cmd;
    QStringList parts;
    mMesh->startMesh(Qtr3dVertexMesh::Triangle);

    enum State {
        SolidState,
        FacetState,
        FacetLoopState,
        VertexState,
        EndState
    };
    /*
        solid ASCII_STL_of_a_sphericon_by_CMG_Lee
        facet normal 0.70675 -0.70746 0
            outer loop
                vertex 1000 0 0
                vertex 0 -1000 0
                vertex 0 -999 -52
            endloop
        endfacet
        endsolid ASCII_STL_of_a_sphericon_by_CMG_Lee
    */

    State state = SolidState;
    QVector3D currentNormal;
    bool parserError = false;



    while (!in.atEnd() && !parserError && (state != EndState))
    {
        line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith("#"))
            continue;

        // process https://en.wikipedia.org/wiki/STL_(file_format)
        parts = line.split(" ");
        if (parts.isEmpty())
            continue;
        cmd = parts.takeFirst().toLower();

        switch (state) {
        case SolidState:               // "solid name"
            if (cmd != "solid") {
                parserError = true;
                break;
            }
            state = FacetState;
            break;
        case FacetState:               // "facet normal ni nj nk"
            if (cmd == "facet") {
                currentNormal = QVector3D();
                if (parts.count() == 4 && parts.takeFirst() == "normal")
                    currentNormal = vectorFromStringList(parts);
                state = FacetLoopState;
                break;
            }
            if (cmd == "endfacet") {
                state = FacetState;
                break;
            }
            if (cmd == "endsolid") {
                state = EndState;
                break;
            }
            parserError = true;
            break;
        case FacetLoopState:
            if (cmd != "outer") {
                parserError = true;
                break;
            }
            state = VertexState;
            break;
        case VertexState:
            if (cmd == "endloop") {
                state = FacetState;
                break;
            }
            if (cmd == "vertex") {
                auto nextVertex = vectorFromStringList(parts);
                if (nextVertex.isNull()) {
                    parserError = true;
                    break;
                }
                if (currentNormal.isNull())
                    mMesh->addVertex(nextVertex);
                else
                    mMesh->addVertex(nextVertex,currentNormal);
                break;
            }
            parserError = true;
            break;
        default: break;
        }
    }

    if (parserError)
        mMesh->reset();
    else
        mMesh->endMesh();

    mMesh = nullptr;

    return !parserError;
}

//-------------------------------------------------------------------------------------------------}
bool Qtr3dPlyLoader::fromBinary(Qtr3dVertexMesh &mesh, QFile &f)
{
    Qtr3dBinReader reader(f.readAll());
    reader.skip(80);
    quint32 numOfTriangles = reader.readUint32();

    mMesh = &mesh;
    mMesh->startMesh(Qtr3dVertexMesh::Triangle);

    while (numOfTriangles > 0) {
        QVector3D normal(reader.readFloat(), reader.readFloat(),reader.readFloat());
        QVector3D v1(reader.readFloat(), reader.readFloat(),reader.readFloat());
        QVector3D v2(reader.readFloat(), reader.readFloat(),reader.readFloat());
        QVector3D v3(reader.readFloat(), reader.readFloat(),reader.readFloat());
        mMesh->addVertex(v1,normal);
        mMesh->addVertex(v2,normal);
        mMesh->addVertex(v3,normal);
        reader.skip(2);
        // quint16 attributSize = reader.readUint16();
        //reader.skip(attributSize);
        numOfTriangles--;
    }

    mMesh->endMesh();

    return true;
}

//-------------------------------------------------------------------------------------------------}

QVector3D Qtr3dPlyLoader::vectorFromStringList(const QStringList &strings)
{
    if (strings.count() != 3)
        return QVector3D();
    bool done;
    double x = strings[0].toDouble(&done); if (!done) return QVector3D();
    double y = strings[1].toDouble(&done); if (!done) return QVector3D();
    double z = strings[2].toDouble(&done); if (!done) return QVector3D();

    return QVector3D(x,y,z);
}

//_------------------------------------------------------------------------------------------------
Qtr3dPlyLoader::AttributType Qtr3dPlyLoader::typeByString(const QString &type)
/*

  http://paulbourke.net/dataformats/ply/

char       character                 1
uchar      unsigned character        1
short      short integer             2
ushort     unsigned short integer    2
int        integer                   4
uint       unsigned integer          4
float      single-precision float    4
double     double-precision float    8
*/
{
    // FIXE: QMap faster?
    if (type == "char")
        return Char;
    if (type == "int8")
        return Char;
    if (type == "uchar")
        return UChar;
    if (type == "uint8")
        return UChar;
    if (type == "short")
        return Short;
    if (type == "ushort")
        return UShort;
    if (type == "int")
        return Int;
    if (type == "int32")
        return Int;
    if (type == "uint")
        return UInt;
    if (type == "uint32")
        return UInt;
    if (type == "float")
        return Float;
    if (type == "float32")
        return Float;
    if (type == "double")
        return Double;
    return InvalidType;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dPlyLoader::decodeAsciiVariant(const QString &inValue, QVariant &outValue, Qtr3dPlyLoader::AttributType t)
{
    switch (t) {
    case Char:
    case UChar:
    case Short:
    case UShort:
    case Int:
    case UInt:   outValue = QVariant(inValue.toLongLong());   return true;
    case Float:
    case Double: outValue = QVariant(inValue.toDouble()); return true;
    default: return false;
    }
    return false;
}

