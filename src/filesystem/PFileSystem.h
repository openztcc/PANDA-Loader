// Project
#include "IVirtualFilesystem.h"

// Qt
#include <QFile>
#include <QDir>

class PFileSystem : public IVirtualFilesystem {
    public:
        PFileSystem() = default;
        ~PFileSystem() override = default;

        // mount point functions
        void setRootPath(const QString &path) override;
        QString rootPath() const override;

        // file operations - relative to root path
        PFileData read(const QString &filePath) override;
        QList<PFileData> readAll(const QStringList& validFolders, const QStringList &validExts) override { return {}; }; // TODO: implement
        bool write(const PFileData &data) override;
        bool remove(const QStringList &itemsToRemove) override;
        bool remove(const QString &itemToRemove);
        bool exists(const QString &filePath) override;

        // move/rename
        bool move(const QString &filePath, const QString &newLocation) override;
        bool copy(const QString &filePath, const QString &newLocation) override;
        bool rename(const QString &filePath, const QString &newFileName) override;

        // directory operations
        // bool makeDir(const QString &dirPath) override;
        // bool dirExists(const QString &dirPath) override;
        // bool removeDir(const QString &dirPath) override;
        // bool listFiles(const QString &dirPath) override;

    private:
        QString m_rootPath; // root path of the zip file
    protected:
        QSharedPointer<QFile> openFile(const QString &filePath, QIODevice::OpenMode mode);
};
