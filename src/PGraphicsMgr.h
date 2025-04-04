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
    static std::vector<OutputBuffer> getGraphicBuffers(const QString &ztdFilePath);
    static QString getPalettePath(const QString &ztdFilePath);
    static bool processIcons(const QString &ztdFilePath);
    static bool convertGraphics(QStringList &iconPaths);
};

#endif // PGRAPHICSMGR_H
