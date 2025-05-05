#ifndef PLauncher_H
#define PLauncher_H

// Qt
#include <QObject>
#include <QProcess>
#include <QString>
#include <QDebug>
#include <QFile>

class PLauncher : public QObject
{
    Q_OBJECT
public:
    PLauncher();
    static bool isIsoMounted(const QString &isoPath);
    static bool mountIso(const QString& isoPath, QString& mountPoint);
    static bool mountIsoIfNeeded(const QString &isoPath);
    // static bool runCommand(const QString &command, const QStringList &args, QString &output, QString &errorOutput="");
    // static QFile getFileFromURL(const QString &url, const QString &fileName = "");
};

#endif // PLauncher_H
