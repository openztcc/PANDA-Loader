#ifndef PSYSTEMMGR_H
#define PSYSTEMMGR_H

#include <QObject>
#include <QProcess>
#include <QString>

class PSystemMgr
{
    Q_OBJECT
public:
    PSystemMgr();
    static bool isIsoMounted(const QString &isoPath) const;
    static bool mountIso(const QString &isoPath) const;
};

#endif // PSYSTEMMGR_H
