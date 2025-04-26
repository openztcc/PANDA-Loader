#ifndef PAPEFILE_H
#define PAPEFILE_H

// Qt
#include <QObject>
#include <QDir>
#include <QTemporaryFile>

// Project
#include "PConfigMgr.h"
#include "PDatabaseMgr.h"
#include "PFile.h"

// External
#include "ApeCore.h"

class PApeFile : public QObject
{
    Q_OBJECT
public:
PApeFile();
    static QMap<QString, OutputBuffer> getGraphicBuffers(const QString &ztdFilePath);
    static QStringList processIcons(QMap<QString, OutputBuffer> &graphicBuffers);
    static bool deleteIcons(const QString &modId);
private:
    QString m_outputiconsPath = QDir::homePath() + "/.panda/modicons/";
};

#endif // PAPEFILE_H
