#ifndef PGRAPHICSMGR_H
#define PGRAPHICSMGR_H

#include <QObject>
#include "ApeCore.h"
#include "PConfigMgr.h"
#include "PZtdMgr.h"

class PGraphicsMgr : public QObject
{
    Q_OBJECT
public:
    PGraphicsMgr();
    static QMap<QString, OutputBuffer> getGraphicBuffers(const QString &ztdFilePath);
    static QStringList graphicBuffers processIcons(QMap<QString, OutputBuffer> &graphicBuffers);
private:
    static QString m_outputiconsPath = QDir::homePath() + "/.panda/modicons/";
};

#endif // PGRAPHICSMGR_H
