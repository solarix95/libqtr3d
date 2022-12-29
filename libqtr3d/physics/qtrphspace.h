#ifndef QTRPHSPACE_H
#define QTRPHSPACE_H

#include <QObject>

class QtrPhEntity;
class QtrPhSpace : public QObject
{
public:
    QtrPhSpace();

public slots:
    void process(int ms);

private:
    QList<QtrPhEntity*> mEntities;
};

#endif // QTRPHSPACE_H
