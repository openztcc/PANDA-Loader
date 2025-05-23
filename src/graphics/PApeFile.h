#ifndef PAPEFILE_H
#define PAPEFILE_H

// Qt
#include <QObject>
#include <QDir>
#include <QTemporaryFile>

// Project
#include "PConfigMgr.h"
#include "PFile.h"

// External
#include "ApeCore.h"

class PApeFile : public QObject
{
    Q_OBJECT
public:
    PApeFile(const QSharedPointer<PFile> &ztd, const QString &outputDir);
    QString generateGraphicAsPng(const QString &graphicPath, const QString &fileName = "");
private:
    QString m_outputiconsPath = QDir::homePath() + "/.panda/modicons/";
    QSharedPointer<PFile> m_ztd;
    QString m_outputDir;
};

#endif // PAPEFILE_H
