#include <QByteArray>
#include <QString>
#include "qtr3dbinreader.h"

#define ERROR(c) if (!c) { mError = true; return 0; }

//-------------------------------------------------------------------------------------------
template <typename T>
T bswap(T val) {
    T retVal;
    char *pVal = (char*) &val;
    char *pRetVal = (char*)&retVal;
    int size = sizeof(T);
    for(int i=0; i<size; i++) {
        pRetVal[size-1-i] = pVal[i];
    }
    return retVal;
}

//-------------------------------------------------------------------------------------------
Qtr3dBinReader::Qtr3dBinReader(const QByteArray &buffer)
    : mCursor(NULL)
    , mParsedBytes(0)
    , mError(false)
    , mByteSwap(false)
{
    if (!buffer.isEmpty())
        setBuffer(buffer);
}

//-------------------------------------------------------------------------------------------
Qtr3dBinReader::Qtr3dBinReader(bool byteSwap, const QByteArray &buffer)
    : mCursor(NULL)
    , mParsedBytes(0)
    , mError(false)
    , mByteSwap(byteSwap)
{
    if (!buffer.isEmpty())
        setBuffer(buffer);
}


//-------------------------------------------------------------------------------------------
void Qtr3dBinReader::setBuffer(const QByteArray &buffer)
{
    mBuffer = buffer;
    mCursor = (quint8*)buffer.constData();
    mParsedBytes = 0;
}

//-------------------------------------------------------------------------------------------
void Qtr3dBinReader::clear()
{
    mCursor = NULL;
    mParsedBytes = 0;
    mBuffer.clear();
}

//-------------------------------------------------------------------------------------------
int Qtr3dBinReader::pos() const
{
    if (!mCursor)
        return -1;
    return mCursor - (quint8*)mBuffer.constData();
}

//-------------------------------------------------------------------------------------------
int Qtr3dBinReader::parsedBytes() const
{
    return mParsedBytes;
}

//-------------------------------------------------------------------------------------------
int Qtr3dBinReader::size() const
{
    return mBuffer.size();
}

//-------------------------------------------------------------------------------------------
bool Qtr3dBinReader::atEnd() const
{
    return !mCursor || (mCursor > tail());
}

//-------------------------------------------------------------------------------------------
bool Qtr3dBinReader::error() const
{
    return mError;
}

//-------------------------------------------------------------------------------------------
bool Qtr3dBinReader::read(void *destPtr, int size)
{
    if ((mCursor + size) > tail())
        return false;
    memcpy(destPtr,mCursor,size);
    mCursor      += size;
    mParsedBytes += size;
    // TODO: ByteSwap
    return true;
}

//-------------------------------------------------------------------------------------------
qint8 Qtr3dBinReader::readInt8()
{
    qint8 ret = 0;
    bool done = read(&ret,1);
    ERROR(done);
    return ret;
}

//-------------------------------------------------------------------------------------------
quint8 Qtr3dBinReader::readUint8()
{
    quint8 ret;
    bool done = read(&ret,1);
    ERROR(done);
    return ret;
}

//-------------------------------------------------------------------------------------------
qint16 Qtr3dBinReader::readInt16()
{
    qint16 ret;
    bool done = read(&ret,2);
    ERROR(done);
    return ret;
}

//-------------------------------------------------------------------------------------------
quint16 Qtr3dBinReader::readUint16()
{
    quint16 ret;
    bool done = read(&ret,2);
    ERROR(done);
    return ret;
}

//-------------------------------------------------------------------------------------------
quint32 Qtr3dBinReader::readUint32()
{
    quint32 ret;
    bool done = read(&ret,4);
    ERROR(done);
    return ret;
}

//-------------------------------------------------------------------------------------------
qint32 Qtr3dBinReader::readInt32()
{
    qint32 ret;
    bool done = read(&ret,4);
    ERROR(done);
    return ret;
}

//-------------------------------------------------------------------------------------------
float Qtr3dBinReader::readFloat()
{
    float ret;
    bool done = read(&ret,4);
    ERROR(done);
    return ret;
}

//-------------------------------------------------------------------------------------------
QString Qtr3dBinReader::readAsciiZ()
{
    char nextChar;
    QByteArray asciiString;
    while ((nextChar = (char)readInt8()) != 0)
        asciiString.append(nextChar);
    return QString::fromLatin1(asciiString);
}

//-------------------------------------------------------------------------------------------
qint32 Qtr3dBinReader::getInt32(int pos) const
{
    Q_ASSERT(pos >= 0 && pos < mBuffer.length());
    return mByteSwap ?
                bswap<qint32>(*(qint32*)(mBuffer.constData() + pos))
                :
                (*(qint32*)(mBuffer.constData() + pos));
}

//-------------------------------------------------------------------------------------------
quint32 Qtr3dBinReader::getUint32(int pos) const
{
    Q_ASSERT(pos >= 0 && pos < mBuffer.length());
    return mByteSwap ?
                bswap<quint32>(*(quint32*)(mBuffer.constData() + pos))
                :
                (*(quint32*)(mBuffer.constData() + pos));
}

//-------------------------------------------------------------------------------------------
bool Qtr3dBinReader::skip(int size)
{
    Q_ASSERT(size >= 0);
    if (size == 0)
        return true;

    if ((mCursor + size) > tail()) {
        mError = true;
        return false;
    }
    mCursor      += size;
    mParsedBytes += size;
    return true;
}

//-------------------------------------------------------------------------------------------
bool Qtr3dBinReader::seek(int pos)
{
    if (pos >= mBuffer.length())
        return false;
    mCursor = (quint8*)mBuffer.constData() + pos;

    return true;
}

//-------------------------------------------------------------------------------------------
QByteArray Qtr3dBinReader::readBlock(int size)
{
    QByteArray block(size,'\0');
    read(block.data(),size);
    return block;
}

//-------------------------------------------------------------------------------------------
quint8 *Qtr3dBinReader::tail() const
{
    Q_ASSERT(mCursor);
    return (quint8*)(mBuffer.constData() + mBuffer.length());
}

