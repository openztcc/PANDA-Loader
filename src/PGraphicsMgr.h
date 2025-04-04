#ifndef PGRAPHICSMGR_H
#define PGRAPHICSMGR_H

#include <QObject>
#include "ApeCore.h"
#include <QDir>
#include <QTemporaryFile>
#include "PConfigMgr.h"

class PGraphicsMgr : public QObject
{
    Q_OBJECT
public:
    PGraphicsMgr();
    static QMap<QString, OutputBuffer> getGraphicBuffers(const QString &ztdFilePath);
    static QStringList processIcons(QMap<QString, OutputBuffer> &graphicBuffers);
private:
    QString m_outputiconsPath = QDir::homePath() + "/.panda/modicons/";
};

#endif // PGRAPHICSMGR_H
