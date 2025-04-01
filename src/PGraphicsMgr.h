#ifndef PGRAPHICSMGR_H
#define PGRAPHICSMGR_H

#include <QObject>
#include "ApeCore.h"
#include "../models/PEntityType.h"

class PGraphicsMgr : public QObject
{
    Q_OBJECT
public:
    PGraphicsMgr();
    static QStringList extractIcons(const QString &ztdFilePath);
    static bool convertGraphics(QStringList &iconPaths);
};

#endif // PGRAPHICSMGR_H
