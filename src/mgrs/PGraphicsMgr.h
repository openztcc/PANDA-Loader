#ifndef PGRAPHICSMGR_H
#define PGRAPHICSMGR_H

// Qt
#include <QObject>
#include <QDir>
#include <QTemporaryFile>

// Project
#include "../ldrs/PConfigMgr.h"
#include "../ldrs/PDatabaseMgr.h"
#include "../ldrs/PFile.h"

// External
#include "ApeCore.h"

class PGraphicsMgr : public QObject
{
    Q_OBJECT
public:
    PGraphicsMgr();
    static QMap<QString, OutputBuffer> getGraphicBuffers(const QString &ztdFilePath);
    static QStringList processIcons(QMap<QString, OutputBuffer> &graphicBuffers);
    static bool deleteIcons(const QString &modId);
private:
    QString m_outputiconsPath = QDir::homePath() + "/.panda/modicons/";
};

#endif // PGRAPHICSMGR_H
