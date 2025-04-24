// Project
#include "../models/PFileData.h"
#include "PZip.h"
#include "PFileSystem.h"
#include "../interfaces/IVirtualFilesystem.h"

// Qt
#include <QCore>

class PFileSystem : public QObject {
    Q_OBJECT
public: 
    PFileSystem() = default;
    ~PFileSystem() override = default;

    bool create(const QString &filePath);

    // mount point functions
    void setRootPath(const QString &path);
    QString rootPath() const;

    PFileData read(const QString &filePath);
    bool write(const QString &filePath, const PFileData &data);
    bool remove(const QString &filePath);
    bool exists(const QString &filePath);
    bool move(const QString &filePath, const QString &newLocation);
    bool copy(const QString &filePath, const QString &newLocation);
    bool rename(const QString &filePath, const QString &newFileName);
    bool replace(const QString &filePath, const PFileData &data);
    bool makeDir(const QString &dirPath);
    bool dirExists(const QString &dirPath);
    bool removeDir(const QString &dirPath);
    bool listFiles(const QString &dirPath);
private:
    std::unique_ptr<IVirtualFilesystem> m_file;System; // pointer to the virtual filesystem
    QString m_rootPath; // root path of the zip file

};