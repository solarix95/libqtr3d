#include <QThread>
#include <QCoreApplication>
#include <QRandomGenerator>
#include "qtr3dutils.h"


qint32 Qtr3d::qrand()
{
    Q_ASSERT(qApp->thread() == QThread::currentThread());
    return QRandomGenerator::global()->generate();

}
