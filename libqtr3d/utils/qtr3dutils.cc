
#include <QThread>
#include <QCoreApplication>

#if QT_VERSION < QT_VERSION_CHECK(5, 10, 0)
#include <QtGlobal>
#else
#include <QRandomGenerator>
#endif

#include "qtr3dutils.h"


qint32 Qtr3d::qrand()
{
    Q_ASSERT(qApp->thread() == QThread::currentThread());

#if QT_VERSION < QT_VERSION_CHECK(5, 10, 0)
    return qrand();
#else
    return QRandomGenerator::global()->generate();
#endif


}
