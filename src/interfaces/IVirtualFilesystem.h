#ifndef IVIRTUALFILESYSTEM_H
#define IVIRTUALFILESYSTEM_H

#include <QCore>

class IVirtualFilesystem {
    public:
        virtual ~IVirtualFilesystem() = default;

        // mount point functions
        virtual void setRootPath(const QString &path) = 0;
        virtual QString rootPath() const = 0;

        // file operations - relative to root path
        virtual QByteArray read(const QString &filePath) = 0;
        virtual bool write(const QString &filePath) = 0;
        virtual bool remove(const QString &filePath) = 0;
        virtual bool exists(const QString &filePath) = 0;

        // move/rename
        virtual bool move(const QString &filePath, const QString &newLocation) = 0;
        virtual bool copy(const QString &filePath, const QString &newLocation) = 0;
        virtual bool rename(const QString &filePath, const QString &newFileName) = 0;
        virtual bool replace(const QString &filePath) = 0;

        // directory operations
        virtual bool makeDir(const QString &dirPath) = 0;
        virtual bool dirExists(const QString &dirPath) = 0;
        virtual bool removeDir(const QString &dirPath) = 0;
        virtual bool listFiles(const QString &dirPath) = 0;
};

#endif // IVIRTUALFILESYSTEM_H