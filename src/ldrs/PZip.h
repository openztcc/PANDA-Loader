// Project
#include "../interfaces/IVirtualFilesystem.h"

// External
#include "quazip.h"
#include "quazipfile.h"

// Qt
#include <QDir>

class PZip : public IVirtualFilesystem {
    public:
        PZip(const QString &filePath = "");
        ~PZip() override = default;

        // mount point functions
        void setRootPath(const QString &path) override;
        QString rootPath() const override;

        // file operations - relative to root path
        PFileData read(const QString &filePath) override;
        bool write(const QString &filePath, const PFileData &data) override;
        bool remove(const QString &filePath) override;
        bool exists(const QString &filePath) override;

        // move/rename
        bool move(const QString &filePath, const QString &newLocation) override;
        bool copy(const QString &filePath, const QString &newLocation) override;
        bool rename(const QString &filePath, const QString &newFileName) override;
        bool replace(const QString &filePath, const PFileData &data) override;

        // directory operations
        bool makeDir(const QString &dirPath) override;
        bool dirExists(const QString &dirPath) override;
        bool removeDir(const QString &dirPath) override;
        bool listFiles(const QString &dirPath) override;

    private:
        QString m_rootPath; // root path of the zip file
    protected:
        QSharedPointer<QuaZip> openZip(const QString &filePath, QuaZip::Mode mode);
        QSharedPointer<QuaZipFile> openZipFile(QSharedPointer<QuaZip> &zip, QIODevice::OpenMode mode);
};
