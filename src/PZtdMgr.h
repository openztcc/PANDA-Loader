#ifndef PZTDMGR
#define PZTDMGR

#include "quazip.h"
#include "quazipfile.h"
#include "quazipnewinfo.h"
#include <QDir>
#include <QUuid>
#include <QBuffer>

class PZtdMgr
{
public:

    // ----------- Local Models ------------------
    // FileData
    struct FileData {
        QByteArray data;
        QString filename;
        QString ext;
        QString path;
    };


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
    static bool isZtdFile(const QString &filePath);
    static bool fileExistsInZtd(const QString &ztdFilePath, const QString &fileNameToCheck);
    static QList<FileData> getFilesInZtd(
        const QString &ztdFilePath,
        const QString &ext,
        int maxLevel = 3,
        const QStringList &folderList = QStringList(),
        QuaZip *zip = nullptr
    );
private:
    static bool extractFilesFromZtd(const QString &ztdFilePath, const QString &tempDir);
    static bool replaceFileInDir(const QString &tempDirPath, const QString &filePathToAdd);
    static bool createZtdFromDir(const QString &ztdFilePath, const QString &tempDirPath);
    static bool removeFileFromDir(const QString &dir);
};

#endif // PZTDMGR
