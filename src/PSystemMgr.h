#ifndef PSYSTEMMGR_H
#define PSYSTEMMGR_H

#include <QObject>
#include <QProcess>
#include <QString>
#include <QDebug>

class PSystemMgr : public QObject
{
    Q_OBJECT
public:
    PSystemMgr();
    static bool isIsoMounted(const QString &isoPath);
    static bool mountIso(const QString& isoPath, QString& mountPoint);
    static bool mountIsoIfNeeded(const QString &isoPath);
};

#endif // PSYSTEMMGR_H
