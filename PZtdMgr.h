#ifndef PZTDMGR
#define PZTDMGR

#include "quazip.h"
#include "quazipfile.h"
#include "quazipnewinfo.h"
#include <QDir>
#include <QUuid>

class PZtdMgr
{
public:
    PZtdMgr();
    ~PZtdMgr();
    static bool hasZtdFiles(const QString &zipFilePath);
    static bool addFileToZtd(const QString &ztdFilePath, const QString &filePathToAdd);
    // TODO: will need below functions for combine/separate feature later
    static bool removeFileFromZtd(const QString &ztdFilePath, const QString &fileNameToRemove);
    // bool listFilesInZtd(const QString &ztdFilePath, QStringList &fileList);
    // bool createZtd(const QString &ztdFilePath, const QStringList &filesToAdd);
    static bool moveZtdFile(const QString &ztdFilePath, const QString &newLocation);
    static bool renameZtdFile(const QString &oldFilePath, const QString &newFileName);
    static bool copyZtdFile(const QString &ztdFilePath, const QString &ztdOutputCopyPath);
    static bool openFileInZtd(const QString &ztdFilePath, const QString &fileNameToOpen, QByteArray &fileData);
    static int isZtdFile(const QString &filePath);
    static bool fileExistsInZtd(const QString &ztdFilePath, const QString &fileNameToCheck);
private:
    static bool extractFilesFromZtd(const QString &ztdFilePath, const QString &tempDir);
    static bool replaceFileInDir(const QString &tempDirPath, const QString &filePathToAdd);
    static bool createZtdFromDir(const QString &ztdFilePath, const QString &tempDirPath);
};

#endif // PZTDMGR
