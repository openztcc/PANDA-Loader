#ifndef PZTDMGR
#define PZTDMGR

#include <quazip/quazip.h>
#include <quazip/quazipfile.h>

class PZtdMgr
{
public:
    PZtdMgr();
    ~PZtdMgr();
    bool hasZtdFiles(const QString &zipFilePath);
    bool addFileToZtd(const QString &ztdFilePath, const QString &filePathToAdd);
    // TODO: will need below functions for combine/separate feature later
    // bool removeFileFromZtd(const QString &ztdFilePath, const QString &fileNameToRemove);
    // bool listFilesInZtd(const QString &ztdFilePath, QStringList &fileList);
    // bool createZtd(const QString &ztdFilePath, const QStringList &filesToAdd);
    bool moveZtdFile(const QString &ztdFilePath, const QString &newLocation);
    bool renameZtdFile(const QString &oldFilePath, const QString &newFileName);
    bool copyZtdFile(const QString &ztdFilePath, const QString &ztdOutputCopyPath);

private:
    bool isZtdFile(const QString &filePath);
};

#endif // PZTDMGR
