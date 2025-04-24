#include "PFile.h"

PFile::PFile(QObject *parent, const QString &filePath, FileType type)
    : QObject(parent), m_rootPath(""), m_file(nullptr) {
    // Initialize the virtual filesystem object
    m_file = createFilesystem(filePath, type);

}

// Create a virtual filesystem object
std::unique_ptr<IVirtualFilesystem> PFile::createFilesystem(const QString &path, FileType type) const
{
    if (type == FileType::Zip) {
        qDebug() << "Creating PZip filesystem for path:" << path;
        return std::make_unique<PZip>(path);
    } else {
        qDebug() << "Creating PFileSystem filesystem for path:" << path;
        return std::make_unique<PFileSystem>();
    }
}

PFileData PFile::read(const QString &relFilePath) {
    // Read the file from the virtual filesystem
    return m_file->read(relFilePath);
}

bool PFile::write(const PFileData &data) {
    // Write the file to the virtual filesystem
    return m_file->write(data);
}

bool PFile::remove(const QString &filePath) {
    // Remove the file from the virtual filesystem
    return m_file->remove(filePath);
}

bool PFile::exists(const QString &filePath) {
    // Check if the file exists in the virtual filesystem
    return m_file->exists(filePath);
}

bool PFile::move(const QString &filePath, const QString &newLocation) {
    // Move the file in the virtual filesystem
    return m_file->move(filePath, newLocation);
}

bool PFile::copy(const QString &filePath, const QString &newLocation) {
    // Copy the file in the virtual filesystem
    return m_file->copy(filePath, newLocation);
}

bool PFile::rename(const QString &filePath, const QString &newFileName) {
    // Rename the file in the virtual filesystem
    return m_file->rename(filePath, newFileName);
}

bool PFile::replace(const QString &filePath, const PFileData &data) {
    // Replace the file in the virtual filesystem
    return m_file->replace(filePath, data);
}

bool PFile::makeDir(const QString &dirPath) {
    // Create a directory in the virtual filesystem
    return m_file->makeDir(dirPath);
}

bool PFile::dirExists(const QString &dirPath) {
    // Check if the directory exists in the virtual filesystem
    return m_file->dirExists(dirPath);
}

bool PFile::removeDir(const QString &dirPath) {
    // Remove the directory from the virtual filesystem
    return m_file->removeDir(dirPath);
}

bool PFile::listFiles(const QString &dirPath) {
    // List the files in the directory in the virtual filesystem
    return m_file->listFiles(dirPath);
}

void PFile::setRootPath(const QString &path) {
    m_rootPath = path;
}

QString PFile::rootPath() const {
    return m_rootPath;
}
