#ifndef BINARYREADER_H
#define BINARYREADER_H

#include <QByteArray>
#include <QString>

class Qtr3dBinReader
{
public:
    Qtr3dBinReader(const QByteArray &buffer = QByteArray());
    Qtr3dBinReader(bool byteSwap, const QByteArray &buffer = QByteArray());

    void setBuffer(const QByteArray &buffer);
    void clear();
    int  pos()  const;
    int  parsedBytes() const;
    int  size() const;
    bool atEnd() const;
    bool error() const;

    bool read(void* destPtr, int size);

    qint8  readInt8();
    quint8 readUint8();
    qint16 readInt16();
    quint16 readUint16();
    quint32 readUint32();
    qint32  readInt32();
    qint64  readInt64();
    quint64 readUint64();
    float   readFloat();
    double  readDouble();
    QString readAsciiZ();

    qint32  getInt32(int pos) const;
    quint32 getUint32(int pos) const;

    bool skip(int bytes);
    bool seek(int pos);
    QByteArray readBlock(int size);


private:
    quint8        *tail() const;

    QByteArray    mBuffer;
    const quint8 *mCursor;
    int           mParsedBytes;
    bool          mError;
    bool          mByteSwap;
};

#endif // BINARYREADER_H
