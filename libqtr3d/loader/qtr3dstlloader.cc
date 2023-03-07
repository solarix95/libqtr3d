#include <QFile>
#include "qtr3dstlloader.h"
#include <libqtr3d/loader/qtr3dbinreader.h>

//-------------------------------------------------------------------------------------------------
bool Qtr3dStlLoader::supportsFile(const QString &filename)
{
    return filename.toLower().endsWith(".stl");
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dStlLoader::loadFile(Qtr3dModel &model, const QString &filename)
{
    Qtr3dStlLoader loader;
    return loader.loadModel(model,filename);
}

//-------------------------------------------------------------------------------------------------
Qtr3dStlLoader::Qtr3dStlLoader()
    : mMesh(nullptr)
{
}

//-------------------------------------------------------------------------------------------------
Qtr3dStlLoader::~Qtr3dStlLoader() = default;

//-------------------------------------------------------------------------------------------------
bool Qtr3dStlLoader::loadModel(Qtr3dModel &model, const QString &filename)
{
    QByteArray header = fileHeader(filename,100);
    if (header.isEmpty())
        return false;

    QFile f(filename);
    if (!f.open(QIODevice::ReadOnly))
        return false;

    auto *mesh = model.context()->createMesh();
    mesh->setDefaultColor(model.material().ambient().mcolor);

    bool done = false;
    if (header.trimmed().startsWith("solid"))
         done = fromASCII(*mesh,f);
    else
        done = fromBinary(*mesh,f);

    if (!done) {
        delete mesh;
        return false;
    }

    model.addMesh(mesh, true);
    return true;
}

//-------------------------------------------------------------------------------------------------
bool Qtr3dStlLoader::fromASCII(Qtr3dMesh &mesh, QFile &f)
{
    QTextStream in(&f);
    mMesh = &mesh;

    QString     line;
    QString     cmd;
    QStringList parts;
    mMesh->startMesh(Qtr3d::Triangle);

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
bool Qtr3dStlLoader::fromBinary(Qtr3dMesh &mesh, QFile &f)
{
    Qtr3dBinReader reader(f.readAll());
    reader.skip(80);
    quint32 numOfTriangles = reader.readUint32();

    mMesh = &mesh;
    mMesh->startMesh(Qtr3d::Triangle);

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

QVector3D Qtr3dStlLoader::vectorFromStringList(const QStringList &strings)
{
    if (strings.count() != 3)
        return QVector3D();
    bool done;
    double x = strings[0].toDouble(&done); if (!done) return QVector3D();
    double y = strings[1].toDouble(&done); if (!done) return QVector3D();
    double z = strings[2].toDouble(&done); if (!done) return QVector3D();

    return QVector3D(x,y,z);
}

