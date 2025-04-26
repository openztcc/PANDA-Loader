#ifndef IVIRTUALFILESYSTEM_H
#define IVIRTUALFILESYSTEM_H

#include <QDir>
#include "PFileData.h"

class IVirtualFilesystem {
    public:
        virtual ~IVirtualFilesystem() = default;

        // mount point functions
        virtual void setRootPath(const QString &path) = 0;
        virtual QString rootPath() const = 0;

        // file operations - relative to root path
        virtual PFileData read(const QString &filePath) = 0;
        virtual QList<PFileData> readAll(const QStringList& validFolders, const QStringList &validExts) = 0;
        virtual bool write(const PFileData &data) = 0;
        virtual bool remove(const QStringList &itemsToRemove) = 0;
        virtual bool exists(const QString &filePath) = 0;

        // move/rename
        virtual bool move(const QString &filePath, const QString &newLocation) = 0;
        virtual bool copy(const QString &filePath, const QString &newLocation) = 0;
        virtual bool rename(const QString &filePath, const QString &newFileName) = 0;

        // directory operations
        // TODO: not needed yet, but implement for future
        // virtual bool makeDir(const QString &dirPath) = 0;
        // virtual bool dirExists(const QString &dirPath) = 0;
        // virtual bool removeDir(const QString &dirPath) = 0;
        // virtual bool listFiles(const QString &dirPath) = 0;
};

#endif // IVIRTUALFILESYSTEM_H
