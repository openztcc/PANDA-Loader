// Project
#include "PFileSystem.h"
#include "PZip.h"

// Qt
#include <QDir>

/*
This is an abstraction layer above the two supported virtual filesystems:
PZip and PFileSystem.
It provides a common interface for file operations such as read, write, remove, etc.
*/

class PFile : public QObject {
    Q_OBJECT
public: 
    PFile(QObject *parent = nullptr, const QString &filePath = "");
    ~PFile() override = default;

    std::unique_ptr<IVirtualFilesystem> createFilesystem(const QString &filePath) const;

    // mount point functions
    void setRootPath(const QString &path);
    QString rootPath() const;
    bool open(const QString &filePath, int mode);
    bool close();

    // file operations - relative to root path
    PFileData read(const QString &filePath);
    bool write(const QString &filePath, const PFileData &data);
    bool remove(const QString &filePath);
    bool exists(const QString &filePath);

    // move/rename
    bool move(const QString &filePath, const QString &newLocation);
    bool copy(const QString &filePath, const QString &newLocation);
    bool rename(const QString &filePath, const QString &newFileName);
    bool replace(const QString &filePath, const PFileData &data);

    // directory operations
    bool makeDir(const QString &dirPath);
    bool dirExists(const QString &dirPath);
    bool removeDir(const QString &dirPath);
    bool listFiles(const QString &dirPath);
private:
    std::unique_ptr<IVirtualFilesystem> m_file; // pointer to the virtual filesystem
    QString m_rootPath; // root path of the zip file

};
