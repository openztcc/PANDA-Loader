#ifndef PSYSTEMMGR_H
#define PSYSTEMMGR_H

#include <QObject>
#include <QProcess>
#include <QString>
#include <QDebug>
#include <QFile>

class PSystemMgr : public QObject
{
    Q_OBJECT
public:
    PSystemMgr();
    static bool isIsoMounted(const QString &isoPath);
    static bool mountIso(const QString& isoPath, QString& mountPoint);
    static bool mountIsoIfNeeded(const QString &isoPath);
    // static bool runCommand(const QString &command, const QStringList &args, QString &output, QString &errorOutput="");
    // static QFile getFileFromURL(const QString &url, const QString &fileName = "");
};

#endif // PSYSTEMMGR_H
