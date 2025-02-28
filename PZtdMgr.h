#ifndef PZTDMGR
#define PZTDMGR

#include <QObject>
#include <quazip/quazip.h>
#include <quazip/quazipfile.h>
#include "PConfigMgr.h"

class PZtdMgr
{
    Q_OBJECT
public:
    PZtdMgr();
    ~PZtdMgr();
    bool extractZtd(const QString &ztdFilePath, const QString &outputDir);
    bool addFileToZtd(const QString &ztdFilePath, const QString &filePathToAdd);
    bool removeFileFromZtd(const QString &ztdFilePath, const QString &fileNameToRemove);
    bool listFilesInZtd(const QString &ztdFilePath, QStringList &fileList);
    bool createZtd(const QString &ztdFilePath, const QStringList &filesToAdd);
    bool moveZtdFile(const QString &ztdFilePath, const QString &fileNameToMove, const QString &newFileName);
    bool renameZtdFile(const QString &ztdFilePath, const QString &oldFileName, const QString &newFileName);
    bool copyZtdFile(const QString &ztdFilePath, const QString &fileNameToCopy, const QString &newFileName);

private:
    PConfigMgr m_configMgr;
    QuaZip m_zip;
    QuaZipFile m_zipFile;

    void closeZtd();
    bool isZtdFile(const QString &filePath);
};

#endif // PZTDMGR
